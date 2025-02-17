#include <stdio.h>
#include <string.h>
#include "text_manipulation.h"

/******************************************************** 
 * Prints a pass or fail message for the specified test.* 
 ********************************************************/
void test_assert(int test_result, const char *test_name, int test_number) {
   if (test_result)  {
      printf("pass %s %d\n", test_name, test_number);
   } else {
      printf("fail %s %d\n", test_name, test_number);
   }
}

int main() {
   char result[MAX_STR_LEN + 1]; 

   int res = right_align("  terp s  ", result, 8);
   test_assert(strcmp(result, " terp s") == 0, "right_align", 1);

   printf("public:&%s&\n", result);
   return 0;
}
