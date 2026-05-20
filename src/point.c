#include "point.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    const char* card_no;
    int points;
} PointMaster;

static const PointMaster kPointMaster[] = {
    {"p0001", 220},
    {"p0002", 500},
    {"p0003", 140},
    {"p0004", 20},
    {"p0005", 5}
};

static int find_points_by_card_no(const char* card_no) {
    int i;
    for (i = 0; i < (int)(sizeof(kPointMaster) / sizeof(kPointMaster[0])); i++) {
        if (strcmp(kPointMaster[i].card_no, card_no) == 0) {
            return kPointMaster[i].points;
        }
    }
    return -1;
}

// ポイントカードの有無を確認（1:持っている, 2:持っていない）
int has_point_card() {
    int sel = 0;
    printf("ポイントカードをお持ちですか？\n[1] 持っている\n[2] 持っていない\n選択: ");
    scanf("%d", &sel);
    while (getchar() != '\n'); // 入力バッファクリア
    return sel;
}

// ポイントカード情報の入力（カード番号入力）
void input_point_card(PointCard* card) {
    printf("カード番号を入力してください: ");
    scanf("%31s", card->card_no);
    while (getchar() != '\n');
    card->usable_points = find_points_by_card_no(card->card_no);
    if (card->usable_points >= 0) {
        card->valid = 1;
    } else {
        card->valid = 0;
        card->usable_points = 0;
    }
}

// ポイント付与
void add_points(PointCard* card, int add) {
    if (card && card->valid) {
        card->usable_points += add;
    }
}

// ポイント利用（成功:1, 失敗:0）
int use_points(PointCard* card, int use) {
    if (card && card->valid && card->usable_points >= use) {
        card->usable_points -= use;
        return 1;
    }
    return 0;
}

// 所持ポイント表示
void show_points(const PointCard* card) {
    if (card && card->valid) {
        printf("現在の所持ポイント: %d\n", card->usable_points);
    }
}