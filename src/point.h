// point.h: ポイントカード情報の定義
#ifndef POINT_H
#define POINT_H

typedef struct {
    char card_no[32];
    int valid;
    int usable_points;
} PointCard;

// ポイントカードの有無を確認
int has_point_card();
// ポイントカード情報の取得（例:カード番号入力）
void input_point_card(PointCard* card);
// ポイント付与
void add_points(PointCard* card, int add);
// ポイント利用
int use_points(PointCard* card, int use);
// 所持ポイント表示
void show_points(const PointCard* card);

#endif // POINT_H