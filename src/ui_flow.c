#include "ui_flow.h"
#include "admin.h"
#include "payment.h"
#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BARCODE_LENGTH 6
#include <windows.h>

// 商品情報構造体とリスト
#define MAX_SCANNED 100
typedef struct {
    char barcode[BARCODE_LENGTH + 1];
    char product_name[50];
    int price;
    int quantity;
} ScannedProduct;
static ScannedProduct scanned_products[MAX_SCANNED];
static int scanned_count = 0;
static int total_price = 0;

void utf8_to_sjis(const char* utf8_str, char* sjis_str, size_t sjis_size);
void use_point_screen(void);

#define BARCODE_LENGTH 6

static int g_need_receipt = 0;

// TODO: 商品読み取り処理と連携したら、現在選択中商品の年齢制限フラグを返す実装に置き換える。
static int needs_age_check_for_current_item(void) {
	return 0;
}

static void age_check_screen(void) {
    int sel = -1, ret;
    printf("\n-------------現在の画面：年齢確認--------------\n");
    printf("\n");
    printf("年齢確認対象の商品です。購入される方は20歳以上ですか？\n");
    printf("\n");
    printf("[1] はい\n");
    printf("[2] いいえ\n");
    printf("\n-----------------------------------------------\n");
    printf("数字を入力     :     ");
    ret = scanf("%d", &sel);
    while (getchar() != '\n');

    if (ret != 1 || (sel != 1 && sel != 2)) {
        printf("無効な入力です。\n");
        age_check_screen();
        return;
    }

    if (sel == 1) {
        printf("年齢確認: はい\n");
    } else {
        printf("年齢確認: いいえ\n");
    }

    point_card_screen();
}

// メインメニュー画面の表示・遷移
void main_menu(void) {
    int sel = -1, ret;
    printf("\n-------------現在の画面：メインメニュー--------------\n");
    printf("\n");
    printf("[1] 読み取り画面\n");
    printf("[9] 管理者メニュー\n");
    printf("\n-----------------------------------------------\n");
    printf("数字を入力     :     ");
    ret = scanf("%d", &sel);
    while (getchar() != '\n');
    if (ret != 1) {
        printf("無効な入力です。\n");
        main_menu();
        return;
    }
    if (sel == 1) {
        scan_screen();
    } else if (sel == 9) {
        admin_login_prompt();
    } else {
        printf("無効な入力です。\n");
        main_menu();
    }
}

// 読み取り画面の表示・遷移
void scan_screen(void) {
    char input[16];
    printf("\n-------------現在の画面：読み取り画面--------------\n\n");
    printf("読み取り済み商品:\n");
    for (int i = 0; i < scanned_count; i++) {
        printf(" %d. %s (%s) %d円 x%d\n", i+1, scanned_products[i].product_name, scanned_products[i].barcode, scanned_products[i].price, scanned_products[i].quantity);
    }
    printf("\n[1] 確認\n[0] 戻る\n");
    printf("\n合計金額: %d円\n", total_price);
    printf("\n-----------------------------------------------\n");
    printf("入力(6桁バーコード / 1:確認 / 0:戻る): ");
    scanf("%15s", input);
    while (getchar() != '\n');

    if (strcmp(input, "1") == 0) {
        printf("確認画面に進みます。\n");
        confirm_screen();
    } else if (strcmp(input, "0") == 0) {
        main_menu();
    } else if (strlen(input) == 6 && strspn(input, "0123456789") == 6) {
        read_barcode_and_display_product(input);
        scan_screen();
    } else {
        printf("無効な入力です。\n");
        scan_screen();
    }
}

// 商品内容確認画面の表示・遷移
void confirm_screen(void) {
    int sel = -1, ret;
    printf("\n-------------現在の画面：確認画面--------------\n");
    printf("\n");
    printf("[1] ポイントカード確認へ\n");
    printf("[0] 戻る\n");
    printf("\n-----------------------------------------------\n");
    printf("数字を入力     :     ");
    ret = scanf("%d", &sel);
    while (getchar() != '\n');
    if (ret != 1) {
        printf("無効な入力です。\n");
        confirm_screen();
        return;
    }
    if (sel == 1) {
        if (needs_age_check_for_current_item()) {
            age_check_screen();
        } else {
            point_card_screen();
        }
    } else if (sel == 0) {
        scan_screen();
    } else {
        printf("無効な入力です。\n");
        confirm_screen();
    }
}

// ポイントカード有無確認画面の表示・遷移
#include "point.h"
static PointCard g_point_card = {0};
static int g_use_point = 0;
void point_card_screen(void) {
    int sel = -1, ret;
    printf("\n-------------現在の画面：ポイントカード確認--------------\n");
    printf("\n");
    printf("[1] ある\n");
    printf("[2] ない\n");
    printf("\n-----------------------------------------------\n");
    printf("数字を入力     :     ");
    ret = scanf("%d", &sel);
    while (getchar() != '\n');
    if (ret != 1) {
        printf("無効な入力です。\n");
        point_card_screen();
        return;
    }
    if (sel == 1) {
        input_point_card(&g_point_card);
        if (g_point_card.valid) {
            printf("ポイントカード認証成功: %s\n", g_point_card.card_no);
            show_points(&g_point_card);
            use_point_screen();
        } else {
            printf("無効なカード番号です。\n");
        }
    } else if (sel == 2) {
        memset(&g_point_card, 0, sizeof(g_point_card));
        payment_method_screen();
    } else {
        printf("無効な入力です。\n");
        point_card_screen();
    }
}

// 支払方法選択画面の表示・遷移
void payment_method_screen(void) {
    int sel = -1, ret;
    printf("\n-------------現在の画面：支払方法選択--------------\n");
    printf("\n");
    printf("[1] 現金\n[2] クレジット\n[3] 電子マネー\n[4] QRコード\n");
    printf("\n-----------------------------------------------\n");
    printf("数字を入力     :     ");
    ret = scanf("%d", &sel);
    while (getchar() != '\n');
    if (ret != 1 || sel < 1 || sel > 4) {
        printf("無効な入力です。\n");
        payment_method_screen();
        return;
    }
    payment_confirm_screen();
}

// 決済確認画面の表示・遷移
void payment_confirm_screen(void) {
    int sel = -1, ret;
    printf("\n-------------現在の画面：決済確認--------------\n");
    printf("\n");
    printf("[1] 確認\n");
    printf("\n-----------------------------------------------\n");
    printf("数字を入力     :     ");
    ret = scanf("%d", &sel);
    while (getchar() != '\n');
    if (ret != 1 || sel != 1) {
        printf("無効な入力です。\n");
        payment_confirm_screen();
        return;
    }
    receipt_option_screen();
}

// レシート発行有無確認画面の表示・遷移
void receipt_option_screen(void) {
    int sel = -1, ret;
    printf("\n-------------現在の画面：レシート発行有無確認--------------\n");
    printf("\n");
    printf("[1] はい\n[2] いいえ\n");
    printf("\n-----------------------------------------------\n");
    printf("数字を入力     :     ");
    ret = scanf("%d", &sel);
    while (getchar() != '\n');
    if (ret != 1 || (sel != 1 && sel != 2)) {
        printf("無効な入力です。\n");
        receipt_option_screen();
        return;
    }
    g_need_receipt = (sel == 1);
    payment_complete_screen();
}

// 決済完了画面の表示・遷移
void payment_complete_screen(void) {
    int sel = -1, ret;
    printf("\n-------------現在の画面：決済完了--------------\n");
    printf("\n");
    if (g_need_receipt) {
        printf("レシートを発行しました。\n");
    }
    printf("[1] 確認\n");
    printf("\n-----------------------------------------------\n");
    printf("数字を入力     :     ");
    ret = scanf("%d", &sel);
    while (getchar() != '\n');
    if (ret != 1 || sel != 1) {
        printf("無効な入力です。\n");
        payment_complete_screen();
        return;
    }

    // ポイント利用分を減算
    if (g_point_card.valid && g_use_point > 0) {
        if (use_points(&g_point_card, g_use_point)) {
            printf("\n%d ポイントを利用しました。\n", g_use_point);
        } else {
            printf("\nポイント利用に失敗しました。\n");
        }
    }
    // ポイント付与処理（例：合計金額の1%）
    if (g_point_card.valid) {
        int after_point = total_price - g_use_point;
        if (after_point < 0) after_point = 0;
        int add = after_point / 100; // 1%
        add_points(&g_point_card, add);
        printf("\nポイントが %d ポイント付与されました。\n", add);
        show_points(&g_point_card);
    }
    g_use_point = 0;

    // Save transaction details to CSV
    const char* employee_id = "22001"; // Example employee ID, replace with actual value
    const char* transaction_details = "Transaction details placeholder"; // Replace with actual transaction details
    save_transaction_to_csv(employee_id, transaction_details);

    g_need_receipt = 0;
    main_menu();
}

// 支払い画面の表示・遷移
void payment_screen(void) {
    int method, amount, ret;

    printf("\n-------------現在の画面：支払い画面--------------\n");
    printf("支払い方法を選択してください:\n");
    printf("[1] 現金\n");
    printf("[2] クレジットカード\n");
    printf("[3] 電子マネー\n");
    printf("[4] QRコード\n");
    printf("[0] 戻る\n");
    printf("-----------------------------------------------\n");
    printf("選択: ");
    ret = scanf("%d", &method);
    while (getchar() != '\n'); // 入力バッファクリア

    if (ret != 1 || method < 0 || method > 4) {
        printf("無効な入力です。\n");
        payment_screen();
        return;
    }

    if (method == 0) {
        printf("メインメニューに戻ります\n");
        main_menu();
        return;
    }

    payment_select_method((PaymentMethod)method);

    printf("支払い金額を入力してください: ");
    ret = scanf("%d", &amount);
    while (getchar() != '\n'); // 入力バッファクリア

    if (ret != 1 || amount <= 0) {
        printf("無効な金額です。\n");
        payment_screen();
        return;
    }

    payment_set_amount(amount);

    if (payment_execute()) {
        printf("支払いが成功しました！\n");
    } else {
        printf("支払いに失敗しました。\n");
    }

    main_menu();
}

// プロトタイプの追加
void save_transaction_to_csv(const char* employee_id, const char* transaction_details);

void show_scan_or_category_screen(void) {
	int sel = -1;
	int ret;
	printf("\n-------------現在の画面：商品スキャン/カテゴリ選択--------------\n");
	printf("\n");
	printf("ここに商品スキャンやカテゴリ選択の説明を記載できます。\n");
	printf("\n\n\n\n\n");
	printf("[1] メインメニューへ    [0] 管理者メニューへ\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel);
	while (getchar() != '\n');
	if (ret != 1) {
		printf("無効な入力です。\n");
		show_scan_or_category_screen();
		return;
	}
	if (sel == 1) {
		printf("メインメニューに遷移\n");
		main_menu();
	} else if (sel == 0) {
		printf("管理者メニューに遷移\n");
		admin_menu();
	} else {
		printf("無効な入力です。\n");
		show_scan_or_category_screen();
	}
}
void show_payment_method_screen(void) {
	int sel = -1;
	int ret;
	printf("\n-------------現在の画面：支払い方法選択--------------\n");
	printf("\n");
	printf("ここに支払い方法選択の説明を記載できます。\n");
	printf("\n\n\n\n\n");
	printf("[1] メインメニューへ    [0] 管理者メニューへ\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel);
	while (getchar() != '\n');
	if (ret != 1) {
		printf("無効な入力です。\n");
		show_payment_method_screen();
		return;
	}
	if (sel == 1) {
		printf("メインメニューに遷移\n");
		main_menu();
	} else if (sel == 0) {
		printf("管理者メニューに遷移\n");
		admin_menu();
	} else {
		printf("無効な入力です。\n");
		show_payment_method_screen();
	}
}
void show_bag_option_screen(void) {
	int sel = -1;
	int ret;
	printf("\n-------------現在の画面：袋オプション--------------\n");
	printf("\n");
	printf("ここに袋オプションの説明を記載できます。\n");
	printf("\n\n\n\n\n");
	printf("[1] メインメニューへ    [0] 管理者メニューへ\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel);
	while (getchar() != '\n');
	if (ret != 1) {
		printf("無効な入力です。\n");
		show_bag_option_screen();
		return;
	}
	if (sel == 1) {
		printf("メインメニューに遷移\n");
		main_menu();
	} else if (sel == 0) {
		printf("管理者メニューに遷移\n");
		admin_menu();
	} else {
		printf("無効な入力です。\n");
		show_bag_option_screen();
	}
}
void show_point_card_screen(void) {
	int sel = -1;
	int ret;
	printf("\n-------------現在の画面：ポイントカード--------------\n");
	printf("\n");
	printf("ここにポイントカードの説明を記載できます。\n");
	printf("\n\n\n\n\n");
	printf("[1] メインメニューへ    [0] 管理者メニューへ\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel);
	while (getchar() != '\n');
	if (ret != 1) {
		printf("無効な入力です。\n");
		show_point_card_screen();
		return;
	}
	if (sel == 1) {
		printf("メインメニューに遷移\n");
		main_menu();
	} else if (sel == 0) {
		printf("管理者メニューに遷移\n");
		admin_menu();
	} else {
		printf("無効な入力です。\n");
		show_point_card_screen();
	}
}
void show_age_check_screen_if_needed(void) {
	int sel = -1;
	int ret;
	printf("\n-------------現在の画面：年齢確認--------------\n");
	printf("\n");
	printf("ここに年齢確認の説明を記載できます。\n");
	printf("\n\n\n\n\n");
	printf("[1] メインメニューへ    [0] 管理者メニューへ\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel);
	while (getchar() != '\n');
	if (ret != 1) {
		printf("無効な入力です。\n");
		show_age_check_screen_if_needed();
		return;
	}
	if (sel == 1) {
		printf("メインメニューに遷移\n");
		main_menu();
	} else if (sel == 0) {
		printf("管理者メニューに遷移\n");
		admin_menu();
	} else {
		printf("無効な入力です。\n");
		show_age_check_screen_if_needed();
	}
}
void show_payment_screen(void) {
	int sel = -1;
	int ret;
	printf("\n-------------現在の画面：支払い--------------\n");
	printf("\n");
	printf("ここに支払いの説明を記載できます。\n");
	printf("\n\n\n\n\n");
	printf("[1] メインメニューへ    [0] 管理者メニューへ\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel);
	while (getchar() != '\n');
	if (ret != 1) {
		printf("無効な入力です。\n");
		show_payment_screen();
		return;
	}
	if (sel == 1) {
		printf("メインメニューに遷移\n");
		main_menu();
	} else if (sel == 0) {
		printf("管理者メニューに遷移\n");
		admin_menu();
	} else {
		printf("無効な入力です。\n");
		show_payment_screen();
	}
}
void show_receipt_option_screen(void) {
	int sel = -1;
	int ret;
	printf("\n-------------現在の画面：レシートオプション--------------\n");
	printf("\n");
	printf("ここにレシートオプションの説明を記載できます。\n");
	printf("\n\n\n\n\n");
	printf("[1] メインメニューへ    [0] 管理者メニューへ\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel);
	while (getchar() != '\n');
	if (ret != 1) {
		printf("無効な入力です。\n");
		show_receipt_option_screen();
		return;
	}
	if (sel == 1) {
		printf("メインメニューに遷移\n");
		main_menu();
	} else if (sel == 0) {
		printf("管理者メニューに遷移\n");
		admin_menu();
	} else {
		printf("無効な入力です。\n");
		show_receipt_option_screen();
	}
}

// 仮の save_transaction_to_csv 関数の実装
void save_transaction_to_csv(const char* employee_id, const char* transaction_details) {
    // TODO: 実際の CSV 保存処理を実装
    printf("[DEBUG] save_transaction_to_csv called with employee_id: %s, transaction_details: %s\n", employee_id, transaction_details);
}

// バーコード入力に基づき、CSVから商品情報を読み取り、画面に表示する処理
void read_barcode_and_display_product(const char* barcode) {
    FILE* file = fopen("data/barcode_products.csv", "r");
    if (!file) {
        printf("CSVファイルを開けませんでした。\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char file_barcode[BARCODE_LENGTH + 1];
        char product_name_utf8[50];
        char product_name_sjis[50];
        int price;

        if (sscanf(line, "%6[^,],%49[^,],%d", file_barcode, product_name_utf8, &price) == 3) {
            if (strcmp(barcode, file_barcode) == 0) {
                // 既にリストにあるか確認
                int found = 0;
                for (int i = 0; i < scanned_count; i++) {
                    if (strcmp(scanned_products[i].barcode, file_barcode) == 0) {
                        scanned_products[i].quantity++;
                        total_price += price;
                        found = 1;
                        printf("商品を追加しました: %s (%d円)\n", scanned_products[i].product_name, price);
                        break;
                    }
                }
                if (!found && scanned_count < MAX_SCANNED) {
                    utf8_to_sjis(product_name_utf8, product_name_sjis, sizeof(product_name_sjis));
                    strcpy(scanned_products[scanned_count].barcode, file_barcode);
                    strcpy(scanned_products[scanned_count].product_name, product_name_sjis);
                    scanned_products[scanned_count].price = price;
                    scanned_products[scanned_count].quantity = 1;
                    total_price += price;
                    printf("商品を追加しました: %s (%d円)\n", product_name_sjis, price);
                    scanned_count++;
                }
                fclose(file);
                return;
            }
        }
    }
    printf("バーコード %s に一致する商品が見つかりませんでした。\n", barcode);
    fclose(file);
}

// UTF-8からShift-JISへの文字コード変換
void utf8_to_sjis(const char* utf8_str, char* sjis_str, size_t sjis_size) {
    WCHAR wide_str[256];
    MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, wide_str, 256);
    WideCharToMultiByte(CP_ACP, 0, wide_str, -1, sjis_str, (int)sjis_size, NULL, NULL);
}

// ポイント利用入力画面
void use_point_screen(void) {
    if (!g_point_card.valid || g_point_card.usable_points <= 0) {
        printf("利用可能なポイントがありません。\n");
        payment_method_screen();
        return;
    }
    printf("利用可能ポイント: %d\n", g_point_card.usable_points);
    printf("利用するポイント数を入力してください（0で利用しない）: ");
    int use = 0;
    scanf("%d", &use);
    while (getchar() != '\n');
    if (use < 0 || use > g_point_card.usable_points) {
        printf("無効なポイント数です。\n");
        use_point_screen();
        return;
    }
    g_use_point = use;
    if (g_use_point > 0) {
        printf("%d ポイントを利用します。\n", g_use_point);
    }
    payment_method_screen();
}