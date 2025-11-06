#include <stdio.h>

void strrev_inplace(char *s){
    char *end = s;
    while(*end != '\0'){
        end++;
    }
    end--;

    char *start = s;
    while(start < end){
        char temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }
}

int main(void) {
	char text1[] = "hello";
    char text2[] = "";
    char text3[] = "A";
    char text4[] = "abba";

    strrev_inplace(text1);
    strrev_inplace(text2);
    strrev_inplace(text3);
    strrev_inplace(text4);
    
    printf("%s\n", text1);
    printf("%s\n", text2);
    printf("%s\n", text3);
    printf("%s\n", text4);

	return 0;
}