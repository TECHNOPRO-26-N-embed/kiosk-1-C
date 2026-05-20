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
    printf("管理者メニュー\n");
}
