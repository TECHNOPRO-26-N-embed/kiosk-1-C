#ifndef PAYMENT_H
#define PAYMENT_H

typedef enum {
    PM_NONE = 0,
    PM_CASH,
    PM_CREDIT,
    PM_EMONEY,
    PM_QR
} PaymentMethod;

#endif // PAYMENT_H
