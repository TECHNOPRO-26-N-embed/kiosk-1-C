# セルフレジ詳細設計（F01-F08）

## 0. 設計方針
- C言語での実装を前提に、画面制御と業務ロジックを分離する。
- 返却値は `ResultCode`（正常/異常）で統一し、詳細は `ErrorCode` で判定する。
- 会計状態は `Transaction` に集約し、明細変更時は必ず `recalc_transaction()` を通す。
- 金額は円単位 `int` で保持する（浮動小数点誤差回避）。税率は百分率（例: 8, 10）で保持。

## 1. 共通データ定義（提案）

```c
#define MAX_PRODUCTS 5000
#define MAX_CART_LINES 200
#define MAX_NAME_LEN 64
#define MAX_BARCODE_LEN 14
#define MAX_CATEGORY 100
#define MAX_HISTORY 10000

typedef enum {
    RC_OK = 0,
    RC_WARN = 1,
    RC_ERR = -1
} ResultCode;

typedef enum {
    ERR_NONE = 0,
    ERR_INVALID_ARG,
    ERR_NOT_FOUND,
    ERR_DUPLICATE,
    ERR_NO_SPACE,
    ERR_INVALID_FORMAT,
    ERR_INVALID_STATE,
    ERR_IO,
    ERR_EMPTY,
    ERR_PAYMENT_REQUIRED,
    ERR_AGE_RESTRICTED
} ErrorCode;

typedef enum {
    PM_NONE = 0,
    PM_CASH,
    PM_CREDIT,
    PM_EMONEY,
    PM_QR
} PaymentMethod;

typedef struct {
    int product_id;
    char name[MAX_NAME_LEN];
    int category_id;
    int unit_price_excl_tax;
    char barcode[MAX_BARCODE_LEN];
    int tax_rate;               // 0, 8, 10 など
    int age_restricted;         // 0: 通常, 1: 年齢確認対象
} Product;

typedef struct {
    Product product_snapshot;   // 会計時点の情報を保持
    int qty;
    int line_subtotal_excl_tax;
    int line_tax;
    int line_total_incl_tax;
} CartLine;

typedef struct {
    int subtotal_excl_tax;
    int total_tax;
    int total_incl_tax;
} Totals;

typedef struct {
    int tax_rate;
    int taxable_amount;
    int tax_amount;
} TaxBreakdown;

typedef struct {
    char card_no[32];
    int valid;
    int usable_points;
} PointCard;

typedef struct {
    CartLine lines[MAX_CART_LINES];
    int line_count;
    PaymentMethod payment_method;
    int need_bag;
    int need_receipt;
    int age_verified;
    int is_confirmed;
    PointCard point_card;
    Totals totals;
} Transaction;

typedef struct {
    Product products[MAX_PRODUCTS];
    int product_count;
    int barcode_index[MAX_PRODUCTS]; // products 配列位置を保持（ソート済み想定）
} ProductMaster;
```

## 2. 商品マスタ管理（F01土台）

### 2.1 init_product_master()
- 目的: 商品マスタ初期化。
- 入力: `ProductMaster* master`
- 出力: `ResultCode`
- アルゴリズム:
1. 引数NULLチェック。
2. `product_count=0`。
3. `products` と `barcode_index` を0クリア。
- エラー処理:
- `master==NULL` は `ERR_INVALID_ARG`。

### 2.2 load_products_from_file(path)
- 目的: CSV等から商品を一括登録。
- 入力: `ProductMaster* master, const char* path`
- 出力: `ResultCode`（読込件数はout引数でも可）
- アルゴリズム:
1. 引数/ファイル存在チェック。
2. 1行ずつ読み込み。
3. `parse` -> `Product` を構築。
4. `validate_product()` 実行。
5. `register_product()` 実行。
6. 最終的に `barcode_index` を検索可能状態へ再構築。
- エラー処理:
- ファイルオープン失敗: `ERR_IO`。
- 形式不正行: スキップして警告件数加算（処理継続）。
- 重複バーコード: スキップして警告。

### 2.3 register_product(product)
- 目的: 商品1件登録。
- 入力: `ProductMaster* master, const Product* product`
- 出力: `ResultCode`
- アルゴリズム:
1. 引数チェック。
2. `validate_product()`。
3. `exists_barcode()` で重複判定。
4. 空き枠確認後 `products[product_count]` に追加。
5. `product_count++`。
6. `barcode_index` を再挿入/再ソート。
- エラー処理:
- 容量超過: `ERR_NO_SPACE`。
- 重複: `ERR_DUPLICATE`。

### 2.4 validate_product(product)
- 目的: 商品妥当性検証。
- 入力: `const Product* product`
- 出力: `ResultCode`
- 検証項目:
- `name` 空文字不可。
- `category_id` 範囲内。
- `unit_price_excl_tax > 0`。
- `tax_rate` は許容値（0/8/10）。
- `barcode` は桁数（8/13等）かつ数字のみ。
- エラー処理:
- 不正項目に応じ `ERR_INVALID_FORMAT` / `ERR_INVALID_ARG`。

### 2.5 find_product_by_barcode(barcode)
- 目的: バーコード高速検索。
- 入力: `const ProductMaster* master, const char* barcode`
- 出力: `const Product*`（見つからない場合NULL）
- アルゴリズム:
1. バーコード形式チェック。
2. `barcode_index` に対して二分探索。
3. 該当インデックスの `products` を返却。
- エラー処理:
- 引数不正/未登録は NULL。

### 2.6 list_products_by_category(category_id, out_list)
- 目的: カテゴリ別一覧。
- 入力: `const ProductMaster* master, int category_id, Product* out_list, int out_cap, int* out_count`
- 出力: `ResultCode`
- アルゴリズム:
1. 引数チェック。
2. 全商品を走査し、カテゴリ一致のみ `out_list` へコピー。
3. `out_count` を設定。
- エラー処理:
- `out_cap` 不足時は `ERR_NO_SPACE`（途中まで格納するか全失敗か方針統一）。

### 2.7 update_product_tax_rate(barcode, new_tax_rate)
- 目的: 税率更新。
- 入力: `ProductMaster* master, const char* barcode, int new_tax_rate`
- 出力: `ResultCode`
- アルゴリズム:
1. 税率検証。
2. バーコード検索。
3. 対象商品の `tax_rate` 更新。
- エラー処理:
- 商品未発見: `ERR_NOT_FOUND`。

### 2.8 exists_barcode(barcode)
- 目的: 重複確認。
- 入力: `const ProductMaster* master, const char* barcode`
- 出力: `int`（1=存在, 0=不存在）
- アルゴリズム:
- `find_product_by_barcode()` をラップ。
- エラー処理:
- 引数不正は0返却 + 呼び出し側でエラー扱い可能にする。

## 3. 商品入力・会計中明細（F01）

### 3.1 start_transaction()
- 目的: 会計開始。
- 入力: `Transaction* tx`
- 出力: `ResultCode`
- アルゴリズム:
1. 引数チェック。
2. `line_count=0`、`payment_method=PM_NONE`。
3. 袋/レシート/年齢確認/確定フラグ初期化。
4. 合計を0初期化。
- エラー処理:
- 引数NULL: `ERR_INVALID_ARG`。

### 3.2 add_item_by_barcode(tx, barcode, qty)
- 目的: バーコード入力追加。
- 入力: `Transaction* tx, const ProductMaster* master, const char* barcode, int qty`
- 出力: `ResultCode`
- アルゴリズム:
1. 状態チェック（確定後編集禁止）。
2. 数量検証。
3. 商品検索。
4. `add_item_to_cart()` 呼び出し。
5. `recalc_transaction()`。
- エラー処理:
- 商品未発見: `ERR_NOT_FOUND`。
- 不正数量: `ERR_INVALID_ARG`。

### 3.3 add_item_by_category_selection(tx, category_id, product_id, qty)
- 目的: カテゴリ経由追加。
- 入力: `Transaction* tx, const ProductMaster* master, int category_id, int product_id, int qty`
- 出力: `ResultCode`
- アルゴリズム:
1. `list_products_by_category()` で候補作成。
2. `product_id` を特定。
3. `add_item_to_cart()` -> `recalc_transaction()`。
- エラー処理:
- カテゴリ不一致やID不正: `ERR_NOT_FOUND`。

### 3.4 add_item_to_cart(tx, product, qty)
- 目的: 共通追加。
- 入力: `Transaction* tx, const Product* product, int qty`
- 出力: `ResultCode`
- アルゴリズム:
1. 既存行探索（同バーコード）。
2. 既存行あり: 数量加算。
3. なし: 新規行追加。
4. 行金額再計算。
- エラー処理:
- 行上限超過: `ERR_NO_SPACE`。
- 数量<=0: `ERR_INVALID_ARG`。

### 3.5 change_item_quantity(tx, line_no, new_qty)
- 目的: 数量訂正。
- 入力: `Transaction* tx, int line_no, int new_qty`
- 出力: `ResultCode`
- アルゴリズム:
1. 行番号範囲チェック。
2. `new_qty==0` なら削除。
3. それ以外は数量更新。
4. `recalc_transaction()`。
- エラー処理:
- 行番号不正: `ERR_INVALID_ARG`。

### 3.6 remove_item_line(tx, line_no)
- 目的: 行取消。
- 入力: `Transaction* tx, int line_no`
- 出力: `ResultCode`
- アルゴリズム:
1. 行番号検証。
2. 後続行を前詰め。
3. `line_count--`。
4. `recalc_transaction()`。
- エラー処理:
- 空カート/行不正: `ERR_EMPTY` / `ERR_INVALID_ARG`。

### 3.7 cancel_last_input(tx)
- 目的: 直前入力取消。
- 入力: `Transaction* tx`
- 出力: `ResultCode`
- アルゴリズム（簡易）:
- 最終行を1件取り消す（または履歴スタック方式）。
- エラー処理:
- 明細無し: `ERR_EMPTY`。

### 3.8 clear_transaction(tx)
- 目的: 会計全取消。
- 入力: `Transaction* tx`
- 出力: `ResultCode`
- アルゴリズム:
- `start_transaction()` を再利用。
- エラー処理:
- NULLのみ。

### 3.9 validate_cart(tx)
- 目的: 会計妥当性。
- 入力: `const Transaction* tx`
- 出力: `ResultCode`
- 検証項目:
- 明細1件以上。
- 全行 `qty>0`。
- 合計が負値でない。
- エラー処理:
- 空明細: `ERR_EMPTY`。

## 4. 税計算・金額計算

### 4.1 calc_line_subtotal(unit_price, qty)
- 入力: `int unit_price_excl_tax, int qty`
- 出力: `int`（税抜小計）
- 式: `unit_price_excl_tax * qty`
- エラー処理:
- オーバーフロー懸念時は上限チェックして `ERR_INVALID_ARG` 相当を返す実装にする。

### 4.2 calc_line_tax(line_subtotal, tax_rate)
- 入力: `int line_subtotal, int tax_rate`
- 出力: `int`（税額）
- 式（四捨五入例）: `(line_subtotal * tax_rate + 50) / 100`
- エラー処理:
- 税率不正は0返却せず、呼び出し側でエラー。

### 4.3 calc_totals(tx, out_totals)
- 目的: 会計全体集計。
- 入力: `const Transaction* tx, Totals* out_totals`
- 出力: `ResultCode`
- アルゴリズム:
1. 各行の税抜/税/税込を合算。
2. `out_totals` 設定。
- エラー処理:
- 引数NULL: `ERR_INVALID_ARG`。

### 4.4 calc_tax_breakdown_by_rate(tx, out_breakdown)
- 目的: 税率別集計。
- 入力: `const Transaction* tx, TaxBreakdown* out_breakdown, int cap, int* out_count`
- 出力: `ResultCode`
- アルゴリズム:
1. 税率ごとにバケット化（0/8/10など固定配列推奨）。
2. 行ごとに加算。
- エラー処理:
- `cap` 不足: `ERR_NO_SPACE`。

### 4.5 recalc_transaction(tx)
- 目的: 明細変更後再計算。
- 入力: `Transaction* tx`
- 出力: `ResultCode`
- アルゴリズム:
1. 各行の税抜/税/税込を再計算。
2. `calc_totals()` 実行。
3. 必要に応じ税率別集計を更新。
- エラー処理:
- 不正税率商品が混在する場合 `ERR_INVALID_STATE`。

## 5. 支払い方法選択（F02, F05）

### 5.1 set_payment_method(tx, method)
- 目的: 支払方法設定（1会計1種類）。
- 入力: `Transaction* tx, PaymentMethod method`
- 出力: `ResultCode`
- アルゴリズム:
1. `method != PM_NONE` チェック。
2. 既設定なら上書き許可可否を判定（要件に合わせ固定）。
- エラー処理:
- 不正 method: `ERR_INVALID_ARG`。

### 5.2 get_payment_method_label(method)
- 入力: `PaymentMethod method`
- 出力: `const char*`
- アルゴリズム:
- `switch` で文言返却。
- エラー処理:
- 未定義値は "不明"。

### 5.3 validate_payment(tx)
- 入力: `const Transaction* tx`
- 出力: `ResultCode`
- 検証:
- `validate_cart()` がOK。
- `payment_method != PM_NONE`。
- エラー処理:
- 未選択: `ERR_PAYMENT_REQUIRED`。

### 5.4 confirm_payment(tx)
- 目的: 最終確定。
- 入力: `Transaction* tx`
- 出力: `ResultCode`
- アルゴリズム:
1. `validate_payment()`。
2. 年齢確認要否判定。
3. 必要時 `age_verified` を確認。
4. `is_confirmed=1`。
5. 履歴保存処理呼び出し。
- エラー処理:
- 年齢未確認: `ERR_AGE_RESTRICTED`。

## 6. 袋・ポイント・年齢確認・レシート（F03, F04, F06, F07）

### 6.1 set_bag_option(tx, need_bag)
- 入力: `Transaction* tx, int need_bag`
- 出力: `ResultCode`
- エラー処理: 0/1以外は `ERR_INVALID_ARG`。

### 6.2 set_receipt_option(tx, need_receipt)
- 入力: `Transaction* tx, int need_receipt`
- 出力: `ResultCode`
- エラー処理: 0/1以外は `ERR_INVALID_ARG`。

### 6.3 read_point_card(card_input, out_card)
- 入力: `const char* card_input, PointCard* out_card`
- 出力: `ResultCode`
- アルゴリズム:
1. カード番号形式チェック。
2. マスタ照会（擬似可）。
3. 有効ならポイント残高設定。
- エラー処理:
- 形式不正/失効: `ERR_INVALID_FORMAT` / `ERR_NOT_FOUND`。

### 6.4 apply_point_card(tx, card)
- 入力: `Transaction* tx, const PointCard* card`
- 出力: `ResultCode`
- アルゴリズム:
- 会計条件（最低購入金額など）を確認して付与/利用設定。
- エラー処理:
- 無効カード: `ERR_INVALID_ARG`。

### 6.5 requires_age_verification(tx)
- 入力: `const Transaction* tx`
- 出力: `int`（1:必要）
- アルゴリズム:
- 明細中の `age_restricted` を1件でも検出したら真。

### 6.6 verify_age(tx, birth_date_or_age, out_result)
- 入力: `Transaction* tx, const char* input, int* out_result`
- 出力: `ResultCode`
- アルゴリズム:
1. 年齢入力または生年月日入力を判別。
2. 年齢計算し閾値（20歳等）判定。
3. 結果を `age_verified` に反映。
- エラー処理:
- 形式不正: `ERR_INVALID_FORMAT`。

## 7. レシート出力（F06）

### 7.1 build_receipt_data(tx, out_receipt)
- 入力: `const Transaction* tx, Receipt* out_receipt`
- 出力: `ResultCode`
- 内容:
- 店舗名、日時、明細、税率別税額、合計、支払方法、袋/レシート情報。
- エラー処理:
- 会計未確定時は `ERR_INVALID_STATE`。

### 7.2 print_receipt(receipt)
- 入力: `const Receipt* receipt`
- 出力: `ResultCode`
- アルゴリズム:
- 印字用固定幅フォーマットで標準出力またはプリンタI/Fへ送信。
- エラー処理:
- 出力失敗は `ERR_IO`。

### 7.3 display_receipt(receipt)
- 入力: `const Receipt* receipt`
- 出力: `ResultCode`
- アルゴリズム:
- 画面幅を意識した整形表示。

### 7.4 format_datetime(now, out_str)
- 入力: `time_t now, char* out_str, size_t out_size`
- 出力: `ResultCode`
- アルゴリズム:
- `localtime` + `strftime("%Y-%m-%d %H:%M:%S")`。
- エラー処理:
- バッファ不足: `ERR_NO_SPACE`。

## 8. 購入履歴（F08）

### 8.1 save_purchase_history(tx)
- 入力: `const Transaction* tx, const char* path`
- 出力: `ResultCode`
- アルゴリズム:
- 1会計1レコードまたは明細展開でCSV追記。
- エラー処理:
- ファイル書込失敗: `ERR_IO`。

### 8.2 load_purchase_history(path)
- 入力: `const char* path, HistoryStore* store`
- 出力: `ResultCode`
- アルゴリズム:
- CSV読込して構造体へ展開。
- エラー処理:
- 形式不正行はスキップ。

### 8.3 search_purchase_history(cond, out_list)
- 入力: `const HistoryStore* store, const SearchCondition* cond, HistoryRecord* out_list, int cap, int* out_count`
- 出力: `ResultCode`
- アルゴリズム:
- 日時範囲、支払方法、商品名部分一致などをAND条件で抽出。

### 8.4 print_history_summary(list)
- 入力: `const HistoryRecord* list, int count`
- 出力: `ResultCode`
- アルゴリズム:
- 日時/件数/金額のサマリを一覧表示。

## 9. 入力画面制御（F01-F07接続）

各画面関数は「表示 + 入力取得 + 次状態返却」を責務にする。

- `show_scan_or_category_screen()`
- 入力: なし
- 出力: 次画面ID
- エラー: 無効入力は再入力ループ。

- `show_payment_method_screen()`
- 出力: `PaymentMethod`

- `show_bag_option_screen()`
- 出力: `need_bag`

- `show_point_card_screen()`
- 出力: カード有無/カード番号

- `show_age_check_screen_if_needed(tx)`
- `requires_age_verification()` が真のときのみ表示。

- `show_payment_screen(tx)`
- 最終金額表示と確定可否の入力。

- `show_receipt_option_screen()`
- レシート要否入力。

## 10. エラー処理・入力検証

### 10.1 parse_barcode(input, out_barcode)
- 数字のみ抽出または完全一致チェック。
- 桁数不正は `ERR_INVALID_FORMAT`。

### 10.2 validate_quantity(input_qty)
- `1 <= qty <= 999` を許容。
- 範囲外は `ERR_INVALID_ARG`。

### 10.3 validate_tax_rate(rate)
- 許容値（0/8/10）以外を拒否。

### 10.4 handle_input_error(code)
- `ErrorCode` ごとにガイド文言を表示。
- 例: `ERR_INVALID_FORMAT` -> "入力形式が正しくありません。再入力してください。"

### 10.5 can_modify_transaction_state(tx)
- `is_confirmed==0` のときのみ編集可。
- 不可時は `ERR_INVALID_STATE`。[13:40]## 11. 推奨モジュール分割
- `product_master.c/h`: 商品マスタ管理。
- `transaction.c/h`: 明細操作・金額計算。
- `payment.c/h`: 支払い関連。
- `option.c/h`: 袋/レシート/年齢/ポイント。
- `receipt.c/h`: レシート生成・表示。
- `history.c/h`: 履歴保存/検索。
- `ui_flow.c/h`: 画面遷移制御。
- `error.c/h`: エラーコードとメッセージ。

## 11. 推奨モジュール分割
- `product_master.c/h`: 商品マスタ管理。
- `transaction.c/h`: 明細操作・金額計算。
- `payment.c/h`: 支払い関連。
- `option.c/h`: 袋/レシート/年齢/ポイント。
- `receipt.c/h`: レシート生成・表示。
- `history.c/h`: 履歴保存/検索。
- `ui_flow.c/h`: 画面遷移制御。
- `error.c/h`: エラーコードとメッセージ。

## 12. 最低限のテスト観点
- バーコード重複登録拒否。
- 数量変更時の再計算整合。
- 税率8%/10%混在の税額内訳。
- 支払方法未選択時の確定拒否。
- 年齢確認対象商品がある会計で未確認拒否。
- 履歴保存/再読込の一致。