#ifndef ERROR_H
#define ERROR_H

typedef enum {
    RC_OK = 0,
    RC_WARN = 1,
    RC_ERR = -1
} ResultCode;

typedef enum {
    ERR_NONE = 0,
    ERR_INVALID_ARG,
    ERR_NOT_FOUND,
    ERR_DUPLICATE,
    ERR_NO_SPACE,
    ERR_INVALID_FORMAT,
    ERR_INVALID_STATE,
    ERR_IO,
    ERR_EMPTY,
    ERR_PAYMENT_REQUIRED,
    ERR_AGE_RESTRICTED
} ErrorCode;

const char* get_error_message(ErrorCode code);

#endif // ERROR_H
