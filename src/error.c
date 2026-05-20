#include "error.h"

const char* get_error_message(ErrorCode code) {
    switch (code) {
        case ERR_NONE: return "正常終了";
        case ERR_INVALID_ARG: return "引数が不正です";
        case ERR_NOT_FOUND: return "見つかりません";
        case ERR_DUPLICATE: return "重複しています";
        case ERR_NO_SPACE: return "容量不足です";
        case ERR_INVALID_FORMAT: return "入力形式が正しくありません";
        case ERR_INVALID_STATE: return "状態が不正です";
        case ERR_IO: return "入出力エラー";
        case ERR_EMPTY: return "データがありません";
        case ERR_PAYMENT_REQUIRED: return "支払い方法が未選択です";
        case ERR_AGE_RESTRICTED: return "年齢確認が必要です";
        default: return "不明なエラー";
    }
}
