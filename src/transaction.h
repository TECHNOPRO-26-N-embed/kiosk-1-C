#ifndef TRANSACTION_H
#define TRANSACTION_H
#include "product.h"
#include "payment.h"
#include "point.h"

// transaction.h: 会計情報・カート明細の定義

// カート1行分
typedef struct {
    Product product_snapshot;   // 商品情報（会計時点）
    int qty;                    // 数量
    int line_subtotal_excl_tax; // 税抜小計
    int line_tax;               // 税額
    int line_total_incl_tax;    // 税込小計
} CartLine;

// 会計合計
typedef struct {
    int subtotal_excl_tax;      // 税抜合計
    int total_tax;              // 税額合計
    int total_incl_tax;         // 税込合計
} Totals;

// 会計全体情報
typedef struct {
    CartLine lines[200];        // カート明細
    int line_count;             // 明細数
    PaymentMethod payment_method; // 支払方法
    int need_bag;               // 袋要否
    int need_receipt;           // レシート要否
    int age_verified;           // 年齢確認済み
    int is_confirmed;           // 会計確定済み
    PointCard point_card;       // ポイントカード情報
    Totals totals;              // 合計金額
} Transaction;

#endif // TRANSACTION_H
