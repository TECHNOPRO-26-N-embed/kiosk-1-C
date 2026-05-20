#ifndef UI_FLOW_H
#define UI_FLOW_H

// 画面遷移制御用関数プロトタイプ

void main_menu(void);
void scan_screen(void);
void confirm_screen(void);
void point_card_screen(void);
void payment_method_screen(void);
void payment_confirm_screen(void);
void receipt_option_screen(void);
void payment_complete_screen(void);
void admin_menu(void);
void read_barcode_and_display_product(const char* barcode);

// プロトタイプの追加
void save_transaction_to_csv(const char* employee_id, const char* transaction_details);

#endif // UI_FLOW_H
