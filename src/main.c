#include <stdio.h>
#include "product.h"
#include "payment.h"
#include "receipt.h"
#include "history.h"
#include "age_check.h"
#include "point.h"
#include "csv.h"
#include "admin.h"
#include "transaction.h"
#include "ui_flow.h"
#include "error.h"

int main() {
    printf("セルフレジシステム起動\n");
    main_menu();
    return 0;
}