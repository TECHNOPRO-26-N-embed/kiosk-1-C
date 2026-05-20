#include "product.h"
#include <string.h>
#include <stdio.h>

// product.c: 商品情報管理の実装
// 商品マスタの初期化や商品登録・検索などを担当

// 商品マスタ初期化
// master: 商品マスタ構造体へのポインタ
void init_product_master(ProductMaster* master) {
    if (!master) return;
    master->product_count = 0;
    memset(master->products, 0, sizeof(master->products));
    memset(master->barcode_index, 0, sizeof(master->barcode_index));
}
