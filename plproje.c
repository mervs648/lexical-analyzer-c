#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {

    /* Kullanıcının komut satırından tam olarak bir argüman (dosya adı) girip girmediğini kontrol ettik */
    if(argc != 2) {
        printf("kullanim: ./plproje <dosya_adi>\n");
        return 1;
    }

    char inFileName[100];
    char outFileName[100];

    /* Girdi ve çıktı dosya adlarını oluşturduk, uzantıları ekledik */
    strcpy(inFileName, argv[1]);
    strcpy(outFileName, argv[1]);

    strcat(inFileName, ".tj");   /* Girdi dosyası: dosyaadi.tj */
    strcat(outFileName, ".lx");  /* Çıktı dosyası: dosyaadi.lx */

    /* Girdi dosyasını okuma, çıktı dosyasını yazma modunda açtık */
    FILE *inputFile = fopen(inFileName, "r");
    FILE *outputFile = fopen(outFileName, "w");

    int ch;

    /* Girdi dosyasının var olup olmadığını kontrol ettik */
    if(inputFile == NULL) {
        printf("hata: girdi dosyasi bulunamadi\n");
        return 1;
    }

    /* Dosyayı karakter karakter okumak için döngü kurduk */
    while((ch = fgetc(inputFile)) != EOF) {

        /* Boşluk, tab ve yeni satır karakterlerini atladık */
        if(isspace(ch)) {
            continue;
        }

        /* Noktalı virgülü satır sonu işareti olarak tanıdık */
        if(ch == ';') {
            fprintf(outputFile, "EndOfLine\n");

        /* Parantezleri tanıdık */
        } else if(ch == '(') {
            fprintf(outputFile, "OpenParanthesis\n");
        } else if(ch == ')') {
            fprintf(outputFile, "CloseParanthesis\n");

        /* Artı ve eksi işaretlerini aritmetik operatör olarak tanıdık */
        } else if(ch == '+' || ch == '-') {
            fprintf(outputFile, "Operator(%c)\n", ch);

        /* := atama operatörünü iki karakter birlikte okuyarak tanıdık */
        } else if(ch == ':') {
            int next_ch = fgetc(inputFile);
            if(next_ch == '=') {
                fprintf(outputFile, "Operator(:=)\n");
            } else {
                printf("lexical error: ':' must be followed by '='\n");
                return 1;
            }

        /* Rakamla başlayan karakterleri tamsayı sabiti olarak tanıdık */
        } else if(isdigit(ch)) {
            char numBuffer[100];
            int i = 0;

            /* Ardışık rakamları buffer'a okuduk */
            while(isdigit(ch)) {
                numBuffer[i] = ch;
                i++;
                ch = fgetc(inputFile);
            }
            numBuffer[i] = '\0';
            fprintf(outputFile, "IntConst(%s)\n", numBuffer);

            /* Rakam olmayan son karakteri geri koyduk */
            ungetc(ch, inputFile);

        /* Harfle başlayan karakterleri identifier veya keyword olarak tanıdık */
        } else if(isalpha(ch)) {
            char wordBuffer[100];
            int i = 0;

            /* Harf, rakam ve alt çizgi içeren karakterleri buffer'a okuduk */
            while(isalnum(ch) || ch == '_') {
                wordBuffer[i] = ch;
                i++;
                ch = fgetc(inputFile);
            }
            wordBuffer[i] = '\0';

            /* Kelimeyi bitiren karakteri geri koyduk */
            ungetc(ch, inputFile);

            /* Identifier uzunluğunun 30 karakteri geçip geçmediğini kontrol ettik */
            if(strlen(wordBuffer) > 30) {
                printf("lexical error: Identifier exceeds 30 characters\n");
                return 1;
            }

            /* Okunan kelimeyi 16 keyword ile karşılaştırdık, eşleşmezse identifier yazdık */
            if(strcmp(wordBuffer, "new") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "int") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "text") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "size") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "subs") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "locate") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "insert") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "override") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "read") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "write") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "from") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "to") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "input") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "output") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "asText") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else if(strcmp(wordBuffer, "asString") == 0) {
                fprintf(outputFile, "Keyword(%s)\n", wordBuffer);
            } else {
                fprintf(outputFile, "Identifier(%s)\n", wordBuffer);
            }

        /* Çift tırnak ile başlayan karakterleri string sabiti olarak tanıdık */
        } else if(ch == '"') {
            char stringBuffer[1000];
            int i = 0;
            ch = fgetc(inputFile);

            /* Kapanış tırnağına veya dosya sonuna kadar karakterleri okuduk */
            while(ch != '"' && ch != EOF) {
                stringBuffer[i] = ch;
                i++;
                ch = fgetc(inputFile);
            }

            /* Dosya bitmeden string kapanmadıysa hata verdik */
            if(ch == EOF) {
                printf("lexical error: unterminated string constant\n");
                return 1;
            }
            stringBuffer[i] = '\0';
            fprintf(outputFile, "StringConst(\"%s\")\n", stringBuffer);

        /* Slash karakterini gördüğümüzde yorum bloğu mu yoksa geçersiz karakter mi olduğunu kontrol ettik */
        } else if(ch == '/') {
            int next_ch = fgetc(inputFile);

            /* Slash'tan sonra yıldız geliyorsa yorum bloğu başlıyor demektir */
            if(next_ch == '*') {
                while(1) {
                    ch = fgetc(inputFile);

                    /* Dosya bitmeden yorum kapanmadıysa hata verdik */
                    if(ch == EOF) {
                        printf("lexical error: unterminated comment\n");
                        return 1;
                    }

                    /* Kapanış işaretini aradık */
                    if(ch == '*') {
                        int lookahead = fgetc(inputFile);
                        if(lookahead == '/') {
                            break; /* Yorum bloğu başarıyla kapandı */
                        } else {
                            ungetc(lookahead, inputFile);
                        }
                    }
                }
            } else {
                /* Slash'tan sonra yıldız gelmezse geçersiz karakter hatası verdik */
                ungetc(next_ch, inputFile);
                printf("lexical error: invalid character /\n");
                return 1;
            }

        /* Hiçbir kurala uymayan karakterler için geçersiz karakter hatası verdik */
        } else {
            printf("lexical error: invalid character %c\n", ch);
            return 1;
        }
    }

    /* İşimiz bitti, her iki dosyayı da kapattık */
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}