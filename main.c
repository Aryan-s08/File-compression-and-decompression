#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void print_menu() {
    printf("+------------------------------------------------------------------+\n");
    printf("|                                                                  |\n");
    printf("|                FILE COMPRESSION AND DECOMPRESSION                |\n");
    printf("|                                                                  |\n");
    printf("+------------------------------------------------------------------+\n");
    printf("1. Compress a file very fast (Huffman compression)\n");
    printf("2. Compress a file fast\n");
    printf("3. Compress a file moderate\n");
    printf("4. Heavy compression\n");
    printf("5. Custom compression\n");
    printf("6. Decompress a file (Huffman)\n");
    printf("7. Decompress a file (LZ77)\n");
    printf("Any other input would exit the program\n");
}

int main() {
    while(1) {
        print_menu();
        int choice;
        int valid = scanf("%d", &choice);
        if(valid != 1 || choice > 7 || choice < 1) {
            printf("\nThank you for using our program.\n");
            return 0;
        }

        switch(choice) {
            case 1:
                if(access("compressh", X_OK) == 0) {
                    int ch = fork();
                    if(ch < 0) {
                        printf("Could not create child process\n");
                        return 0;
                    }
                    else if(ch == 0) {
                        printf("Enter filename: ");
                        char filename[1000];
                        if(scanf("%s", filename) == 1) {
                            execl("compressh", "compressh", filename, NULL);
                        }
                        else {
                            printf("Invalid filename\n");
                        }
                        return 0;
                    }
                    else {
                        wait(NULL);
                    }
                }
                else {
                    printf("Executable permission denied.\n");
                }
                break;
            case 2:
                if(access("compress", X_OK) == 0) {
                    int ch = fork();
                    if(ch < 0) {
                        printf("Could not create child process\n");
                        return 0;
                    }
                    else if(ch == 0) {
                        printf("Enter filename: ");
                        char filename[1000];
                        if(scanf("%s", filename) == 1) {
                            execl("compress", "compress", filename, "3", "8", NULL);
                        }
                        else {
                            printf("Invalid filename\n");
                        }
                        return 0;
                    }
                    else {
                        wait(NULL);
                    }
                }
                else {
                    printf("Executable permission denied.\n");
                }
                break;
            case 3:
                if(access("compress", X_OK) == 0) {
                    int ch = fork();
                    if(ch < 0) {
                        printf("Could not create child process\n");
                        return 0;
                    }
                    else if(ch == 0) {
                        printf("Enter filename: ");
                        char filename[1000];
                        if(scanf("%s", filename) == 1) {
                            execl("compress", "compress", filename, "4", "14", NULL);
                        }
                        else {
                            printf("Invalid filename\n");
                        }
                        return 0;
                    }
                    else {
                        wait(NULL);
                    }
                }
                else {
                    printf("Executable permission denied.\n");
                }
                break;
            case 4:
                if(access("compress", X_OK) == 0) {
                    int ch = fork();
                    if(ch < 0) {
                        printf("Could not create child process\n");
                        return 0;
                    }
                    else if(ch == 0) {
                        printf("Enter filename: ");
                        char filename[1000];
                        if(scanf("%s", filename) == 1) {
                            execl("compress", "compress", filename, "6", "20", NULL);
                        }
                        else {
                            printf("Invalid filename\n");
                        }
                        return 0;
                    }
                    else {
                        wait(NULL);
                    }
                }
                else {
                    printf("Executable permission denied.\n");
                }
                break;
            case 5:
                if(access("compress", X_OK) == 0) {
                    int ch = fork();
                    if(ch < 0) {
                        printf("Could not create child process\n");
                        return 0;
                    }
                    else if(ch == 0) {
                        printf("Enter filename max_characters max_distance: ");
                        char filename[1000], arg1[20], arg2[20];
                        if(scanf("%s %s %s", filename, arg1, arg2) == 3) {
                            execl("compress", "compress", filename, arg1, arg2, NULL);
                        }
                        else {
                            printf("Invalid filename\n");
                        }
                        return 0;
                    }
                    else {
                        wait(NULL);
                    }
                }
                else {
                    printf("Executable permission denied.\n");
                }
                break;
                break;
            case 6:
                if(access("decompressh", X_OK) == 0) {
                    int ch = fork();
                    if(ch < 0) {
                        printf("Could not create child process\n");
                        return 0;
                    }
                    else if(ch == 0) {
                        printf("Enter filename: ");
                        char filename[1000];
                        if(scanf("%s", filename) == 1) {
                            execl("decompressh", "decompressh", filename, NULL);
                        }
                        else {
                            printf("Invalid filename\n");
                        }
                        return 0;
                    }
                    else {
                        wait(NULL);
                    }
                }
                else {
                    printf("Executable permission denied.\n");
                }
                break;
            case 7:
                if(access("decompress", X_OK) == 0) {
                    int ch = fork();
                    if(ch < 0) {
                        printf("Could not create child process\n");
                        return 0;
                    }
                    else if(ch == 0) {
                        printf("Enter filename: ");
                        char filename[1000];
                        if(scanf("%s", filename) == 1) {
                            execl("decompress", "decompress", filename, NULL);
                        }
                        else {
                            printf("Invalid filename\n");
                        }
                        return 0;
                    }
                    else {
                        wait(NULL);
                    }
                }
                else {
                    printf("Executable permission denied.\n");
                }
                break;
            default:
                printf("\nThank you for using our program..\n");
                return 0;
        }
    }
}