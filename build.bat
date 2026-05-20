@echo off
echo.
echo === コンパイル中 ===
gcc -finput-charset=utf-8 -fexec-charset=cp932 src\main.c src\product.c src\payment.c src\receipt.c src\history.c src\age_check.c src\point.c src\csv.c src\admin.c src\transaction.c src\ui_flow.c src\error.c -o app.exe
if %ERRORLEVEL% EQU 0 (
    echo.
    echo [成功] app.exe が生成されました
    echo 実行するには以下を入力してください:
    echo   .\app.exe
    echo.
) else (
    echo.
    echo [エラー] コンパイルに失敗しました
    echo 上のエラーメッセージを確認してください
    echo.
)
