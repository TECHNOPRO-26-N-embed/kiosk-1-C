#ifndef PRODUCT_H
#define PRODUCT_H

<<<<<<< Updated upstream
// product.h: 商品情報の管理用ヘッダ
// 商品情報の構造体定義や商品マスタ管理用の構造体を提供
=======
#include "error.h"

#define MAX_PRODUCTS 5000
#define MAX_NAME_LEN 64
#define MAX_BARCODE_LEN 14
#define MAX_CATEGORY 100
>>>>>>> Stashed changes

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

// 商品マスタ初期化・クリア
ResultCode init_product_master(ProductMaster* master);
ResultCode clear_product_master(ProductMaster* master);

// 商品検証
ResultCode validate_product(const Product* product, ErrorCode* out_error);
int validate_tax_rate(int tax_rate);
int validate_barcode(const char* barcode);

// 商品登録・存在確認
ResultCode register_product(ProductMaster* master, const Product* product, ErrorCode* out_error);
int exists_product_id(const ProductMaster* master, int product_id);
int exists_barcode(const ProductMaster* master, const char* barcode);

// 商品検索
Product* find_product_by_id(ProductMaster* master, int product_id);
const Product* find_product_by_id_const(const ProductMaster* master, int product_id);
Product* find_product_by_barcode(ProductMaster* master, const char* barcode);
const Product* find_product_by_barcode_const(const ProductMaster* master, const char* barcode);

// 一覧・更新
ResultCode list_products_by_category(
    const ProductMaster* master,
    int category_id,
    Product* out_list,
    int out_cap,
    int* out_count,
    ErrorCode* out_error
);
ResultCode update_product_tax_rate(ProductMaster* master, const char* barcode, int new_tax_rate, ErrorCode* out_error);

// CSV入出力
ResultCode load_products_from_file(
    ProductMaster* master,
    const char* path,
    int* out_loaded,
    int* out_skipped,
    ErrorCode* out_error
);
ResultCode save_products_to_file(const ProductMaster* master, const char* path, ErrorCode* out_error);

#endif // PRODUCT_H
