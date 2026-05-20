#ifndef PAYMENT_H
#define PAYMENT_H

// 支払方法（指定どおり）
typedef enum {
    PM_NONE = 0,
    PM_CASH,
    PM_CREDIT,
    PM_EMONEY,
    PM_QR
} PaymentMethod;

// 決済状態
typedef enum {
    PS_IDLE = 0,       // 未開始
    PS_SELECTED,       // 支払方法選択済み
    PS_PROCESSING,     // 処理中
    PS_SUCCESS,        // 成功
    PS_FAILED          // 失敗
} PaymentStatus;

// 決済情報管理構造体
typedef struct {
    PaymentMethod method;
    PaymentStatus status;
    int amount;
} PaymentInfo;

// 初期化
void payment_init(void);

// 支払方法選択
void payment_select_method(PaymentMethod method);

// 金額設定
void payment_set_amount(int amount);

// 決済実行
int payment_execute(void);

#endif