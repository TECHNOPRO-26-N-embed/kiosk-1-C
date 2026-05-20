
// point.h: ポイントカード情報の定義
#ifndef POINT_H
#define POINT_H

typedef struct {
    char card_no[32];
    int valid;
    int usable_points;
} PointCard;

#endif // POINT_H
