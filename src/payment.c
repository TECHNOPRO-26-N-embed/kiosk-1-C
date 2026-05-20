
#include "payment.h"
#include <stdio.h>

// 内部状態
static PaymentInfo g_payment;

// 初期化
void payment_init(void) {
    g_payment.method = PM_NONE;
    g_payment.status = PS_IDLE;
    g_payment.amount = 0;
}

// 支払方法設定
void payment_select_method(PaymentMethod method) {
    g_payment.method = method;
    g_payment.status = PS_SELECTED;
}

// 金額設定
void payment_set_amount(int amount) {
    g_payment.amount = amount;
}

// 状態取得
PaymentStatus payment_get_status(void) {
    return g_payment.status;
}

// 方法取得
PaymentMethod payment_get_method(void) {
    return g_payment.method;
}

// 決済処理
int payment_execute(void) {
    if (g_payment.method == PM_NONE) {
        printf("支払方法が選択されていません\n");
        g_payment.status = PS_FAILED;
        return 0;
    }

    if (g_payment.amount <= 0) {
        printf("金額が不正です\n");
        g_payment.status = PS_FAILED;
        return 0;
    }

    g_payment.status = PS_PROCESSING;

    printf("\n=== 決済処理開始 ===\n");

    switch (g_payment.method) {
        case PM_CASH:
            printf("現金決済処理中...\n");
            break;
        case PM_CREDIT:
            printf("クレジット認証中...\n");
            break;
        case PM_EMONEY:
            printf("電子マネー残高確認中...\n");
            break;
        case PM_QR:
            printf("QRコード決済実行中...\n");
            break;
        default:
            printf("不明な支払方法\n");
            g_payment.status = PS_FAILED;
            return 0;
    }

    // 疑似的に成功させる
    g_payment.status = PS_SUCCESS;

    printf("=== 決済成功 ===\n");

    return 1;
}

// リセット
void payment_reset(void) {
    payment_init();
}

// デバッグ表示
void payment_debug_print(void) {
    printf("\n--- 決済情報 ---\n");
    printf("金額: %d\n", g_payment.amount);

    printf("支払方法: ");
    switch (g_payment.method) {
        case PM_CASH:   printf("現金\n"); break;
        case PM_CREDIT: printf("クレジット\n"); break;
        case PM_EMONEY: printf("電子マネー\n"); break;
        case PM_QR:    printf("QRコード\n"); break;
        default:        printf("未選択\n"); break;
    }

    printf("状態: ");
    switch (g_payment.status) {
        case PS_IDLE:     printf("未開始\n"); break;
        case PS_SELECTED: printf("選択済み\n"); break;
        case PS_PROCESSING: printf("処理中\n"); break;
        case PS_SUCCESS:  printf("成功\n"); break;
        case PS_FAILED:   printf("失敗\n"); break;
    }

    printf("----------------\n");
}