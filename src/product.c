#include "product.h"
#include <string.h>
#include <stdio.h>

// product.c: 商品情報管理の実装
// 商品マスタの初期化や商品登録・検索などを担当

// 商品マスタ初期化
<<<<<<< Updated upstream
// master: 商品マスタ構造体へのポインタ
void init_product_master(ProductMaster* master) {
    if (!master) return;
=======
ResultCode init_product_master(ProductMaster* master) {
    if (!master) return RC_ERR;
>>>>>>> Stashed changes
    master->product_count = 0;
    memset(master->products, 0, sizeof(master->products));
    memset(master->barcode_index, 0, sizeof(master->barcode_index));
    return RC_OK;
}

ResultCode clear_product_master(ProductMaster* master) {
    return init_product_master(master);
}
