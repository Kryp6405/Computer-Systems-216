#include <stdio.h>
#include <stdlib.h>

int draw_rectangle(int w, int l, char c);
int draw_triangle(int s, char c);
int valid_character(char c);

int main(){
    int input;
    do{
        printf("Enter 1(rectangle), 2(triangle), 3(other), 0(quit): ");
        scanf("%d", &input);
        switch(input){
            case 0:
                printf("Bye Bye.\n");
                break;
            case 1:
                {
                    int width, length;
                    char rect_char;
                    printf("Enter character, width and length: ");
                    scanf(" %c %d %d", &rect_char, &width, &length);

                    if(draw_rectangle(width, length, rect_char) == 0)
                        printf("Invalid data provided.\n");
                    break;
                }
            case 2:
                {
                    int size;
                    char tri_char;
                    printf("Enter character and size: ");
                    scanf(" %c %d", &tri_char, &size);

                    if(draw_triangle(size, tri_char) == 0)
                        printf("Invalid data provided.\n");
                    break;
                }
            case 3:
                printf("\n");
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    } while(input != 0);
    return 0;
}

int draw_rectangle(int w, int l, char c){
    int i;
    if(w <= 0 || l <= 0 || !valid_character(c))
        return 0;
    for(i = w; i > 0; i--){
        int j;
        for(j = l; j > 0; j--)
            printf("%c", c);
        printf("\n");
    }
    return 1;
}

int draw_triangle(int s, char c){
    int i;
    if(s <= 0 || !valid_character(c))
        return 0;
    for(i = 0; i < s; i++){
        int j, k;
        for(j = i; j < s-1; j++)
            printf(" ");
        for(k = 0; k < 2*(i+1)-1; k++)
            printf("%c", c);
        printf("\n");
    }
    return 1;
}

int valid_character(char c){
    return (c == '*' || c == '%' || c == '#');
}