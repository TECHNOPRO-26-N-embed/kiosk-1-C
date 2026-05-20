#include "product.h"
#include <string.h>
#include <stdio.h>

// 商品マスタ初期化
void init_product_master(ProductMaster* master) {
    if (!master) return;
    master->product_count = 0;
    memset(master->products, 0, sizeof(master->products));
    memset(master->barcode_index, 0, sizeof(master->barcode_index));
}
