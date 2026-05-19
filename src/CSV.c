#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int CSV(void)
{
    int data[3][3]={
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };
    FILE *fp=fopen("test.csv","w");
       if (fp == NULL) {
        perror("ファイルオープン失敗");
        return EXIT_FAILURE;
    }
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            fprintf(fp,"%d", data[i][j]);

            if(j<2)
            {
                fprintf(fp,",");
            }
        }
        fprintf(fp,"\n");

    }
    fclose(fp);
    return EXIT_SUCCESS;
}

char *CSV_Scene()
{
       
    int sceneNumber=0;//ここはシーンが何番に移行するのかを記憶する変数
    //表示部分
    printf("管理者画面\n");
    printf("========================= メニュー =========================\n");
    printf("\n\n\n\n\n");





    printf("[0]スタート画面へ\t\t\t[1]CSVに出力");
    printf("\n");
    printf("=============================================================\n");

    printf("番号を入力\t:\t");
    scanf("%d",&sceneNumber);


    if(sceneNumber==0)
    {
        return "Start";//ここでシーンの名前を入れる
    }
    else if(sceneNumber==1)
    {
        CSV();
        return "CSV";
    }
    else
    {
        printf("無効な選択です\n");
    }
  


    return "CSV";//シーンの名前を返す
}



