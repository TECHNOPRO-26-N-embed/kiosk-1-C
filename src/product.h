#ifndef PRODUCT_H
#define PRODUCT_H

// product.h: 商品情報の管理用ヘッダ
// 商品情報の構造体定義や商品マスタ管理用の構造体を提供

#define MAX_PRODUCTS 5000      // 商品最大数
#define MAX_NAME_LEN 64        // 商品名最大長
#define MAX_BARCODE_LEN 14     // バーコード最大長
#define MAX_CATEGORY 100       // カテゴリ最大数

// 商品1件分の情報
typedef struct {
    int product_id;                   // 商品ID
    char name[MAX_NAME_LEN];          // 商品名
    int category_id;                  // カテゴリID
    int unit_price_excl_tax;          // 税抜単価
    char barcode[MAX_BARCODE_LEN];    // バーコード
    int tax_rate;                     // 税率（0,8,10など）
    int age_restricted;               // 年齢確認対象か（0:通常, 1:要確認）
} Product;

// 商品マスタ全体
typedef struct {
    Product products[MAX_PRODUCTS];   // 商品配列
    int product_count;                // 登録商品数
    int barcode_index[MAX_PRODUCTS];  // バーコード検索用インデックス
} ProductMaster;

#endif // PRODUCT_H
