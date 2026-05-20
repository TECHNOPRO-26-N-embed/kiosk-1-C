#ifndef PRODUCT_H
#define PRODUCT_H

#include "error.h"

// 最大商品数・文字長などの定義
#define MAX_PRODUCTS 5000
#define MAX_NAME_LEN 64
#define MAX_BARCODE_LEN 14
#define MAX_CATEGORY 100

// 商品情報構造体
typedef struct {
    int product_id;                        // 商品ID
    char name[MAX_NAME_LEN];               // 商品名
    int category_id;                       // カテゴリID
    int unit_price_excl_tax;               // 税抜単価
    char barcode[MAX_BARCODE_LEN];         // バーコード（数字文字列）
    int tax_rate;                          // 税率（0,8,10など）
    int age_restricted;                    // 年齢制限（0:なし, 1:あり）
} Product;

// 商品マスタ構造体
typedef struct {
    Product products[MAX_PRODUCTS];        // 商品配列
    int product_count;                     // 登録商品数
    int barcode_index[MAX_PRODUCTS];       // バーコード検索用インデックス
} ProductMaster;

// 商品マスタ初期化・クリア
ResultCode init_product_master(ProductMaster* master); // 商品マスタを初期化
ResultCode clear_product_master(ProductMaster* master); // 商品マスタをクリア

// 商品データのバリデーション
ResultCode validate_product(const Product* product, ErrorCode* out_error); // 商品データの妥当性検証
int validate_tax_rate(int tax_rate); // 税率の検証
int validate_barcode(const char* barcode); // バーコードの検証

// 商品登録・存在確認
ResultCode register_product(ProductMaster* master, const Product* product, ErrorCode* out_error); // 商品登録
int exists_product_id(const ProductMaster* master, int product_id); // 商品ID重複チェック
int exists_barcode(const ProductMaster* master, const char* barcode); // バーコード重複チェック

// 商品検索
Product* find_product_by_id(ProductMaster* master, int product_id); // 商品IDで検索
const Product* find_product_by_id_const(const ProductMaster* master, int product_id); // 商品IDで検索（const）
Product* find_product_by_barcode(ProductMaster* master, const char* barcode); // バーコードで検索
const Product* find_product_by_barcode_const(const ProductMaster* master, const char* barcode); // バーコードで検索（const）

// カテゴリ別一覧・税率更新
ResultCode list_products_by_category(const ProductMaster* master, int category_id, Product* out_list, int out_cap, int* out_count, ErrorCode* out_error); // カテゴリ別一覧
ResultCode update_product_tax_rate(ProductMaster* master, const char* barcode, int new_tax_rate, ErrorCode* out_error); // 税率更新

// CSV入出力
ResultCode load_products_from_file(ProductMaster* master, const char* path, int* out_loaded, int* out_skipped, ErrorCode* out_error); // CSV読込
ResultCode save_products_to_file(const ProductMaster* master, const char* path, ErrorCode* out_error); // CSV書出

#endif // PRODUCT_H