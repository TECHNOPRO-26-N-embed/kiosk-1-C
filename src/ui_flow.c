#include "ui_flow.h"
#include "admin.h"
#include "payment.h"
#include "point.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif

static int g_need_receipt = 0;
// グローバル変数として合計金額を保持
static int g_total_price = 0;

#define TEMP_PRODUCTS_MAX 200
#define TEMP_SCANNED_MAX 100

typedef struct {
    char barcode[7];
    char name[64];
    int price;
} TempProduct;

static TempProduct g_temp_products[TEMP_PRODUCTS_MAX];
static int g_temp_product_count = 0;
static int g_temp_products_loaded = 0;

static const TempProduct* g_scanned_items[TEMP_SCANNED_MAX];
static int g_scanned_count = 0;

static int is_six_digit_barcode(const char* s) {
    int i;
    if (s == NULL || strlen(s) != 6) {
        return 0;
    }
    for (i = 0; i < 6; i++) {
        if (!isdigit((unsigned char)s[i])) {
            return 0;
        }
    }
    return 1;
}

static void trim_newline(char* s) {
    size_t len;
    if (s == NULL) {
        return;
    }
    len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}

static void normalize_name_for_console(char* s) {
#ifdef _WIN32
    wchar_t wide_buf[128];
    char local_buf[128];
    int wide_len;
    int local_len;

    if (s == NULL || s[0] == '\0') {
        return;
    }

    wide_len = MultiByteToWideChar(
        CP_UTF8,
        0,
        s,
        -1,
        wide_buf,
        (int)(sizeof(wide_buf) / sizeof(wide_buf[0]))
    );
    if (wide_len <= 0) {
        return;
    }

    local_len = WideCharToMultiByte(CP_ACP, 0, wide_buf, -1, local_buf, (int)sizeof(local_buf), NULL, NULL);
    if (local_len <= 0) {
        return;
    }

    strncpy(s, local_buf, 63);
    s[63] = '\0';
#else
    (void)s;
#endif
}

static void load_temp_products_csv(void) {
    FILE* fp;
    char line[256];

    if (g_temp_products_loaded) {
        return;
    }

    fp = fopen("data/barcode_products.csv", "r");
    if (fp == NULL) {
        g_temp_products_loaded = 1;
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        char* comma1;
        char* comma2;
        char* barcode;
        char* name;
        char* price_text;
        long price;

        if (g_temp_product_count >= TEMP_PRODUCTS_MAX) {
            break;
        }

        trim_newline(line);
        if (line[0] == '\0') {
            continue;
        }

        comma1 = strchr(line, ',');
        if (comma1 == NULL) {
            continue;
        }
        *comma1 = '\0';
        barcode = line;
        name = comma1 + 1;

        comma2 = strchr(name, ',');
        if (comma2 == NULL) {
            continue;
        }
        *comma2 = '\0';
        price_text = comma2 + 1;

        if (!is_six_digit_barcode(barcode) || name[0] == '\0') {
            continue;
        }

        price = strtol(price_text, NULL, 10);
        if (price < 0) {
            continue;
        }

        strncpy(g_temp_products[g_temp_product_count].barcode, barcode, 6);
        g_temp_products[g_temp_product_count].barcode[6] = '\0';
        strncpy(g_temp_products[g_temp_product_count].name, name, 63);
        g_temp_products[g_temp_product_count].name[63] = '\0';
        g_temp_products[g_temp_product_count].price = (int)price;
        normalize_name_for_console(g_temp_products[g_temp_product_count].name);
        g_temp_product_count++;
    }

    fclose(fp);
    g_temp_products_loaded = 1;
}

static const TempProduct* find_temp_product_by_barcode(const char* barcode) {
    int i;
    for (i = 0; i < g_temp_product_count; i++) {
        if (strcmp(g_temp_products[i].barcode, barcode) == 0) {
            return &g_temp_products[i];
        }
    }
    return NULL;
}

static void reset_scanned_items(void) {
    g_scanned_count = 0;
}

static void add_scanned_item(const TempProduct* product) {
    if (product == NULL) {
        return;
    }
    if (g_scanned_count >= TEMP_SCANNED_MAX) {
        return;
    }
    g_scanned_items[g_scanned_count] = product;
    g_scanned_count++;
}

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
        reset_scanned_items();
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
    char input[64];
    const TempProduct* product;
    int i;
    int total = 0;

    load_temp_products_csv();

    printf("\n-------------現在の画面：読み取り画面--------------\n");
    printf("\n");
    if (g_scanned_count == 0) {
        printf("読み取り済み商品: なし\n");
    } else {
        printf("読み取り済み商品:\n");
        for (i = 0; i < g_scanned_count; i++) {
            printf("%2d. %s (%s) %d円\n", i + 1, g_scanned_items[i]->name, g_scanned_items[i]->barcode, g_scanned_items[i]->price);
            total += g_scanned_items[i]->price;
        }
        g_total_price = total; // グローバル変数に保存
    }
    printf("\n");
    printf("[1] 確認\n");
    printf("[0] 戻る\n");
    printf("\n合計金額: %d円\n", total);
    printf("\n-----------------------------------------------\n");
    printf("入力(6桁バーコード / 1:確認 / 0:戻る): ");
    scanf("%63s", input);
    while (getchar() != '\n');

    if (strcmp(input, "1") == 0) {
        confirm_screen();
    } else if (strcmp(input, "0") == 0) {
        main_menu();
    } else if (is_six_digit_barcode(input)) {
        product = find_temp_product_by_barcode(input);
        if (product != NULL) {
            add_scanned_item(product);
            printf("商品を追加しました: %s (%d円)\n", product->name, product->price);
        } else {
            printf("該当する商品が見つかりません。\n");
        }
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
        point_card_screen();
    } else if (sel == 0) {
        scan_screen();
    } else {
        printf("無効な入力です。\n");
        confirm_screen();
    }
}

// ポイントカード確認画面の表示・遷移
void point_card_screen(void) {
    int sel = -1, ret;
    printf("\n-------------現在の画面：ポイントカード確認--------------\n");
    printf("\n");
    printf("[1] ある\n"); // ポイントカードあり
    printf("[2] ない\n"); // ポイントカードなし
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
        PointCard card = {{0}, 0, 0};
        int use = 0;
        int max_use = 0;

        input_point_card(&card);
        if (!card.valid) {
            printf("カード番号が無効です。再入力を促します。\n");
            point_card_screen();
            return;
        }

        printf("カード番号が有効です。所持ポイントを表示します。\n");
        show_points(&card);
        max_use = card.usable_points;
        printf("利用可能ポイント上限: %d\n", max_use);
        printf("何ポイント使いますか？ (0〜%d): ", max_use);
        ret = scanf("%d", &use);
        while (getchar() != '\n');

        if (ret != 1 || use < 0 || use > max_use) {
            printf("無効なポイント利用入力です。再入力を促します。\n");
            point_card_screen();
            return;
        }

        if (!use_points(&card, use)) {
            printf("ポイント利用に失敗しました。再入力を促します。\n");
            point_card_screen();
            return;
        }

        g_total_price -= use; // 合計金額を更新
        printf("%dポイント利用しました。\n", use);
        printf("ポイント利用後の合計金額: %d円\n", g_total_price); // 更新後の合計金額を表示

        payment_method_screen();
    } else if (sel == 2) {
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