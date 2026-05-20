#ifndef PRODUCT_H
#define PRODUCT_H

#define MAX_PRODUCTS 5000
#define MAX_NAME_LEN 64
#define MAX_BARCODE_LEN 14
#define MAX_CATEGORY 100

typedef struct {
    int product_id;
    char name[MAX_NAME_LEN];
    int category_id;
    int unit_price_excl_tax;
    char barcode[MAX_BARCODE_LEN];
    int tax_rate; // 0, 8, 10 など
    int age_restricted; // 0: 通常, 1: 年齢確認対象
} Product;

typedef struct {
    Product products[MAX_PRODUCTS];
    int product_count;
    int barcode_index[MAX_PRODUCTS];
} ProductMaster;

#endif // PRODUCT_H
