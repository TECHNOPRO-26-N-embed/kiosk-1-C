#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"

#define MAX_LINE_LENGTH 1024

// CSVファイルを読み込む関数
int input_csv(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // 改行を削除
        line[strcspn(line, "\r\n")] = 0;

        // カンマで分割
        char* token = strtok(line, ",");
        while (token) {
            printf("%s\n", token); // デバッグ用に出力
            token = strtok(NULL, ",");
        }
    }

    fclose(file);
    return 0;
}
