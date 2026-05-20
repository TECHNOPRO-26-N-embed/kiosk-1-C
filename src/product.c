#include "product.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// 商品マスタを初期化
ResultCode init_product_master(ProductMaster* master) {
    if (!master) return RC_ERR;
    master->product_count = 0;
    memset(master->products, 0, sizeof(master->products));
    memset(master->barcode_index, 0, sizeof(master->barcode_index));
    return RC_OK;
}

// 商品マスタをクリア（初期化と同じ動作）
ResultCode clear_product_master(ProductMaster* master) {
    return init_product_master(master);
}

// 商品データの妥当性検証
ResultCode validate_product(const Product* product, ErrorCode* out_error) {
    if (!product) {
        if (out_error) *out_error = ERR_INVALID_ARG;
        return RC_ERR;
    }
    if (strlen(product->name) == 0) {
        if (out_error) *out_error = ERR_INVALID_FORMAT;
        return RC_ERR;
    }
    if (product->category_id < 0 || product->category_id >= MAX_CATEGORY) {
        if (out_error) *out_error = ERR_INVALID_FORMAT;
        return RC_ERR;
    }
    if (product->unit_price_excl_tax <= 0) {
        if (out_error) *out_error = ERR_INVALID_FORMAT;
        return RC_ERR;
    }
    if (!validate_tax_rate(product->tax_rate)) {
        if (out_error) *out_error = ERR_INVALID_FORMAT;
        return RC_ERR;
    }
    if (!validate_barcode(product->barcode)) {
        if (out_error) *out_error = ERR_INVALID_FORMAT;
        return RC_ERR;
    }
    return RC_OK;
}

// 税率の検証（0,8,10のみ許容）
int validate_tax_rate(int tax_rate) {
    return (tax_rate == 0 || tax_rate == 8 || tax_rate == 10);
}

// バーコードの検証（8桁または13桁の数字のみ）
int validate_barcode(const char* barcode) {
    if (!barcode) return 0;
    size_t len = strlen(barcode);
    if (!(len == 8 || len == 13)) return 0;
    for (size_t i = 0; i < len; ++i) {
        if (!isdigit((unsigned char)barcode[i])) return 0;
    }
    return 1;
}

// 商品ID重複チェック
int exists_product_id(const ProductMaster* master, int product_id) {
    if (!master) return 0;
    for (int i = 0; i < master->product_count; ++i) {
        if (master->products[i].product_id == product_id) return 1;
    }
    return 0;
}

// バーコード重複チェック
int exists_barcode(const ProductMaster* master, const char* barcode) {
    return find_product_by_barcode_const(master, barcode) != NULL;
}

// 商品登録
ResultCode register_product(ProductMaster* master, const Product* product, ErrorCode* out_error) {
    if (!master || !product) {
        if (out_error) *out_error = ERR_INVALID_ARG;
        return RC_ERR;
    }
    if (master->product_count >= MAX_PRODUCTS) {
        if (out_error) *out_error = ERR_NO_SPACE;
        return RC_ERR;
    }
    if (exists_barcode(master, product->barcode)) {
        if (out_error) *out_error = ERR_DUPLICATE;
        return RC_ERR;
    }
    ErrorCode v_err;
    if (validate_product(product, &v_err) != RC_OK) {
        if (out_error) *out_error = v_err;
        return RC_ERR;
    }
    master->products[master->product_count] = *product;
    master->product_count++;
    return RC_OK;
}

// 商品IDで検索
Product* find_product_by_id(ProductMaster* master, int product_id) {
    if (!master) return NULL;
    for (int i = 0; i < master->product_count; ++i) {
        if (master->products[i].product_id == product_id) return &master->products[i];
    }
    return NULL;
}

const Product* find_product_by_id_const(const ProductMaster* master, int product_id) {
    if (!master) return NULL;
    for (int i = 0; i < master->product_count; ++i) {
        if (master->products[i].product_id == product_id) return &master->products[i];
    }
    return NULL;
}

// バーコードで検索（線形探索）
Product* find_product_by_barcode(ProductMaster* master, const char* barcode) {
    if (!master || !barcode) return NULL;
    for (int i = 0; i < master->product_count; ++i) {
        if (strcmp(master->products[i].barcode, barcode) == 0) return &master->products[i];
    }
    return NULL;
}

const Product* find_product_by_barcode_const(const ProductMaster* master, const char* barcode) {
    if (!master || !barcode) return NULL;
    for (int i = 0; i < master->product_count; ++i) {
        if (strcmp(master->products[i].barcode, barcode) == 0) return &master->products[i];
    }
    return NULL;
}

// カテゴリ別一覧取得
ResultCode list_products_by_category(const ProductMaster* master, int category_id, Product* out_list, int out_cap, int* out_count, ErrorCode* out_error) {
    if (!master || !out_list || !out_count) {
        if (out_error) *out_error = ERR_INVALID_ARG;
        return RC_ERR;
    }
    int cnt = 0;
    for (int i = 0; i < master->product_count; ++i) {
        if (master->products[i].category_id == category_id) {
            if (cnt >= out_cap) {
                if (out_error) *out_error = ERR_NO_SPACE;
                *out_count = cnt;
                return RC_ERR;
            }
            out_list[cnt++] = master->products[i];
        }
    }
    *out_count = cnt;
    return RC_OK;
}

// 税率更新
ResultCode update_product_tax_rate(ProductMaster* master, const char* barcode, int new_tax_rate, ErrorCode* out_error) {
    if (!master || !barcode) {
        if (out_error) *out_error = ERR_INVALID_ARG;
        return RC_ERR;
    }
    if (!validate_tax_rate(new_tax_rate)) {
        if (out_error) *out_error = ERR_INVALID_FORMAT;
        return RC_ERR;
    }
    Product* p = find_product_by_barcode(master, barcode);
    if (!p) {
        if (out_error) *out_error = ERR_NOT_FOUND;
        return RC_ERR;
    }
    p->tax_rate = new_tax_rate;
    return RC_OK;
}

// CSV読込・書出は未実装（雛形のみ）
ResultCode load_products_from_file(ProductMaster* master, const char* path, int* out_loaded, int* out_skipped, ErrorCode* out_error) {
    // TODO: CSVファイルから商品を読み込む実装
    return RC_ERR;
}

ResultCode save_products_to_file(const ProductMaster* master, const char* path, ErrorCode* out_error) {
    // TODO: 商品マスタをCSVファイルに保存する実装
    return RC_ERR;
}