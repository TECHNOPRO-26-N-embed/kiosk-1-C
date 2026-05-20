// main.c: プログラムのエントリーポイント
#include <stdio.h>
#include "product.h"      // 商品管理
#include "payment.h"      // 支払い処理
#include "receipt.h"      // レシート処理
#include "history.h"      // 購入履歴
#include "age_check.h"    // 年齢確認
#include "point.h"        // ポイントカード
#include "csv.h"          // CSV入出力
#include "admin.h"        // 管理者機能
#include "transaction.h"  // 会計情報
#include "ui_flow.h"      // 画面遷移・UI
#include "error.h"        // エラー処理

int main() {
    // セルフレジシステムの起動メッセージ
    printf("セルフレジシステム起動\n");
    // 最初にメインメニュー画面を表示
    main_menu();
    return 0;
}