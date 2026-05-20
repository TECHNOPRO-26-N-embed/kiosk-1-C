#include "admin.h"
#include <stdio.h>
#include <string.h>

// admin.c: 管理者メニュー・認証処理の実装（仮）

// 従業員コード入力画面
void admin_login_prompt(void) {
    char code[32];
    int valid = 0;
    printf("従業員コードを入力してください: ");
    scanf("%31s", code);
    // 仮認証: 指定コードのみ許可
    if (
        strcmp(code, "22001") == 0 ||
        strcmp(code, "22002") == 0 ||
        strcmp(code, "22003") == 0 ||
        strcmp(code, "21004") == 0
    ) {
        valid = 1;
    }
    if (valid) {
        printf("\n");
        admin_menu();
    } else {
        printf("認証失敗: コードが正しくありません\n\n");
    }
}

void admin_menu(void) {
    int sel = -1;
    printf("\n-------------管理者メニュー--------------\n");
    printf("[1] 商品管理\n");
    printf("[0] 戻る\n");
    printf("-----------------------------------------\n");
    printf("選択してください: ");
    scanf("%d", &sel);
    while (getchar() != '\n');

    switch (sel) {
        case 1:
            product_management_menu();
            break;
        case 0:
            printf("メインメニューに戻ります。\n");
            break;
        default:
            printf("無効な選択です。\n");
            admin_menu();
            break;
    }
}

void product_management_menu(void) {
    printf("\n-------------商品管理メニュー--------------\n");
    printf("[1] 商品一覧表示\n");
    printf("[2] 商品追加\n");
    printf("[3] 商品削除\n");
    printf("[0] 戻る\n");
    printf("-------------------------------------------\n");
    printf("選択してください: ");
    int sel;
    scanf("%d", &sel);
    while (getchar() != '\n');

    switch (sel) {
        case 1:
            printf("商品一覧を表示します。\n");
            break;
        case 2:
            printf("商品を追加します。\n");
            break;
        case 3:
            printf("商品を削除します。\n");
            break;
        case 0:
            admin_menu();
            break;
        default:
            printf("無効な選択です。\n");
            product_management_menu();
            break;
    }
}
