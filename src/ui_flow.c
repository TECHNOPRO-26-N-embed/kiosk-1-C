#include "ui_flow.h"
#include "admin.h"
#include <stdio.h>

// メインメニュー画面の表示・遷移
void main_menu(void) {
	int sel = -1, ret; // sel: ユーザー入力値, ret: scanfの戻り値
	printf("\n-------------現在の画面：メインメニュー--------------\n");
	printf("\n");
	printf("[1] 読み取り画面\n"); // 商品スキャン等のメイン処理へ
	printf("[9] 管理者メニュー\n"); // 管理者用画面へ
	printf("\n\n\n\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel); // 入力受付
	while (getchar() != '\n'); // 入力バッファクリア
	if (ret != 1) { // 入力が数値でない場合
		printf("無効な入力です。\n");
		main_menu();
		return;
	}
	if (sel == 1) { // 読み取り画面へ
		printf("読み取り画面に遷移\n");
		scan_screen();
	} else if (sel == 9) { // 管理者メニューへ
		printf("管理者メニューに遷移\n");
		admin_menu();
	} else { // その他は無効
		printf("無効な入力です。\n");
		main_menu();
	}
}

// 読み取り画面の表示・遷移
void scan_screen(void) {
	int sel = -1, ret;
	printf("\n-------------現在の画面：読み取り画面--------------\n");
	printf("\n");
	printf("[1] 確認\n"); // 次の確認画面へ
	printf("[0] 戻る\n"); // メインメニューへ戻る
	printf("\n\n\n\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel); // 入力受付
	while (getchar() != '\n'); // 入力バッファクリア
	if (ret != 1) { // 入力が数値でない場合
		printf("無効な入力です。\n");
		scan_screen();
		return;
	}
	if (sel == 1) { // 確認画面へ
		printf("確認画面に遷移\n");
		confirm_screen();
	} else if (sel == 0) { // メインメニューへ
		printf("メインメニューに戻ります\n");
		main_menu();
	} else { // その他は無効
		printf("無効な入力です。\n");
		scan_screen();
	}
}

// 商品内容確認画面の表示・遷移
void confirm_screen(void) {
	int sel = -1, ret;
	printf("\n-------------現在の画面：確認画面--------------\n");
	printf("\n");
	printf("[1] ポイントカード確認へ\n"); // ポイントカード確認画面へ
	printf("[0] 戻る\n"); // 読み取り画面へ戻る
	printf("\n\n\n\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel); // 入力受付
	while (getchar() != '\n'); // 入力バッファクリア
	if (ret != 1) { // 入力が数値でない場合
		printf("無効な入力です。\n");
		confirm_screen();
		return;
	}
	if (sel == 1) { // ポイントカード確認画面へ
		printf("ポイントカード確認画面に遷移\n");
		point_card_screen();
	} else if (sel == 0) { // 読み取り画面へ
		printf("読み取り画面に戻ります\n");
		scan_screen();
	} else { // その他は無効
		printf("無効な入力です。\n");
		confirm_screen();
	}
}

// ポイントカード有無確認画面の表示・遷移
void point_card_screen(void) {
	int sel = -1, ret;
	printf("\n-------------現在の画面：ポイントカード確認--------------\n");
	printf("\n");
	printf("[1] ある\n"); // ポイントカードあり
	printf("[2] ない\n"); // ポイントカードなし
	printf("\n\n\n\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel); // 入力受付
	while (getchar() != '\n'); // 入力バッファクリア
	if (ret != 1) { // 入力が数値でない場合
		printf("無効な入力です。\n");
		point_card_screen();
		return;
	}
	if (sel == 1) { // ポイントカードあり（今は未実装）
		printf("ポイントカード処理（未実装）\n");
		payment_method_screen();
	} else if (sel == 2) { // ポイントカードなし→支払方法選択へ
		printf("支払方法選択画面に遷移\n");
		payment_method_screen();
	} else { // その他は無効
		printf("無効な入力です。\n");
		point_card_screen();
	}
}

// 支払方法選択画面の表示・遷移
void payment_method_screen(void) {
	int sel = -1, ret;
	printf("\n-------------現在の画面：支払方法選択--------------\n");
	printf("\n");
	printf("[1] 現金\n[2] クレジット\n[3] 電子マネー\n[4] QRコード\n"); // 支払方法選択肢
	printf("\n\n\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel); // 入力受付
	while (getchar() != '\n'); // 入力バッファクリア
	if (ret != 1 || sel < 1 || sel > 4) { // 1～4以外は無効
		printf("無効な入力です。\n");
		payment_method_screen();
		return;
	}
	printf("決済確認画面に遷移\n");
	payment_confirm_screen();
}

// 決済確認画面の表示・遷移
void payment_confirm_screen(void) {
	int sel = -1, ret;
	printf("\n-------------現在の画面：決済確認--------------\n");
	printf("\n");
	printf("[1] 確認\n"); // 決済確定
	printf("\n\n\n\n\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel); // 入力受付
	while (getchar() != '\n'); // 入力バッファクリア
	if (ret != 1 || sel != 1) { // 1以外は無効
		printf("無効な入力です。\n");
		payment_confirm_screen();
		return;
	}
	printf("レシート発行有無確認画面に遷移\n");
	receipt_option_screen();
}

// レシート発行有無確認画面の表示・遷移
void receipt_option_screen(void) {
	int sel = -1, ret;
	printf("\n-------------現在の画面：レシート発行有無確認--------------\n");
	printf("\n");
	printf("[1] はい\n[2] いいえ\n"); // レシート発行有無
	printf("\n\n\n\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel); // 入力受付
	while (getchar() != '\n'); // 入力バッファクリア
	if (ret != 1 || (sel != 1 && sel != 2)) { // 1,2以外は無効
		printf("無効な入力です。\n");
		receipt_option_screen();
		return;
	}
	printf("決済完了画面に遷移\n");
	payment_complete_screen();
}

// 決済完了画面の表示・遷移
void payment_complete_screen(void) {
	int sel = -1, ret;
	printf("\n-------------現在の画面：決済完了--------------\n");
	printf("\n");
	printf("[1] 確認\n"); // メインメニューに戻る
	printf("\n\n\n\n\n");
	printf("-----------------------------------------------\n");
	printf("数字を入力     :     ");
	ret = scanf("%d", &sel); // 入力受付
	while (getchar() != '\n'); // 入力バッファクリア
	if (ret != 1 || sel != 1) { // 1以外は無効
		printf("無効な入力です。\n");
		payment_complete_screen();
		return;
	}
	printf("メインメニューに戻ります\n");
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
