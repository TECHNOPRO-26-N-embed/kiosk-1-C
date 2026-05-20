#ifndef TRANSACTION_H
#define TRANSACTION_H
#include "product.h"
#include "payment.h"
#include "point.h"

typedef struct {
    Product product_snapshot;
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
    CartLine lines[200];
    int line_count;
    PaymentMethod payment_method;
    int need_bag;
    int need_receipt;
    int age_verified;
    int is_confirmed;
    PointCard point_card;
    Totals totals;
} Transaction;

#endif // TRANSACTION_H
