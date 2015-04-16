#include <stdio.h>
#include <wchar.h>
#include <windows.h>
int total = 0; // total chinese characters

// UNICODE version word counter
void word_count_u(FILE* pf)
{
    int w = 0, b = 2;
    wint_t c;

    while((c = getwc(pf)) != WEOF) {
        b += 2;         // byte count
        if (c > 127) {  // 中文字符
            w++;        // char count
        }
    }
   
    printf("%10d \t %10d\n", w, b);
    total += w;
}

// word counter
void word_count(const char* file)
{
    int w = 0, b = 0;
    int c;
    int unicode = 0;
   
    FILE *pf = fopen(file, "rb");
    if (NULL == pf) {
        return;
    }
    printf ("%20s : ", file);
    
    // 判断是否为 UNICODE 文件
    if ((c = getc(pf)) == 0xff) {
        int cc;
        if ((cc = getc(pf)) == 0xfe) {
            unicode = 1;
            printf("UNICODE");
            word_count_u(pf);
        }
        else {
            fseek(pf, 0, SEEK_SET);
        }
    }
    else {
        ungetc(c, pf);
    }
   
    if (!unicode) {
        printf("       ");
        while((c = getc(pf)) != EOF) {
            b++; // byte count
            if (c > 127) { // 中文字符
                w++; // char count
                b++; // 每个中文字符占两字节
                if ((c = getc(pf)) == EOF)
                    break;
            }
        }
   
        printf("%10d \t %10d\n", w, b);
        total += w;
    }
    fclose(pf);
}

void count_files(const char* file)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile(file, &FindFileData);

    if (INVALID_HANDLE_VALUE == hFind) {
        fprintf(stderr, "Can't find %s\n", file);
        return ;
    }

    do {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {
            continue;
        }
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // do nothing
        }
        else {
            word_count(FindFileData.cFileName);
        }
    } while (FindNextFile(hFind, &FindFileData));

    FindClose(hFind);
}

int main(int argc, char* argv[])
{
    if (1 == argc) {
        // default: count all .tex and .txt files in current dir
        count_files("*.tex");
        count_files("*.txt");
    }
    else {
        // get filenames from command line arguments
        while (--argc > 0) {
            count_files(argv[argc]);
        }
    }
    printf("  Total : %d\n", total);
	system("pause");
    return 0;
}