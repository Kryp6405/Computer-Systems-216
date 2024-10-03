/*
Krisnajit Rajeshkhanna
UID: 119769843
Directory ID: krajeshk
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <ctype.h>
#include "document.h"

int user_add_paragraph_after(Document *doc, char *request) {
   char command[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
   int num_of_val, para_num;

   num_of_val = sscanf(request, "%s%d%s", command, &para_num, extra);

   if(num_of_val == 2 && para_num >= 0) {
      if(add_paragraph_after(doc, para_num) == FAILURE) {
	      printf("add_paragraph_after failed\n");
      }
      return SUCCESS;
   }
   return FAILURE;
}

int user_add_line_after(Document *doc, char *request) {
   char command[MAX_STR_SIZE + 1], input_str[MAX_STR_SIZE + 1];
   int num_of_val, para_num, line_num;

   num_of_val = sscanf(request, "%s%d%d%s", command, &para_num, &line_num, input_str);
   
   if(num_of_val == 4 && para_num > 0 && line_num >= 0) {
      char line[MAX_STR_SIZE + 1];
      
      if(strchr(request, '*') != NULL) {
	      strcpy(line, strchr(request, '*') + 1);
	 
         if(add_line_after(doc, para_num, line_num, line) == FAILURE) {
            printf("add_line_after failed\n");
         }

	      return SUCCESS;
      }
   }

   return FAILURE;
}

int user_print_document(Document *doc, char *request) {
   char command[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
   int num_of_val = sscanf(request, "%s%s", command, extra);

   if(num_of_val == 1) {
      if(print_document(doc) == FAILURE) {
	      printf("print_document failed\n");
      }

      return SUCCESS;
   }

   return FAILURE;
}

int user_append_line(Document *doc, char *request) {
   char command[MAX_STR_SIZE + 1], input_str[MAX_STR_SIZE + 1];
   int num_of_val, para_num;

   num_of_val = sscanf(request, "%s%d%s", command, &para_num, input_str);

   if(num_of_val == 3 && para_num > 0) {
      char line[MAX_STR_SIZE + 1];

      if(strchr(request, '*') != NULL) {
	      strcpy(line, strchr(request, '*') + 1);

         if(append_line(doc, para_num, line) == FAILURE) {
            printf("append_line failed\n");
         }
         
         return SUCCESS;
      }
   }

   return FAILURE;
}

int user_remove_line(Document *doc, char *request) {
   char command[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
   int num_of_val, para_line, line_num;

   num_of_val = sscanf(request, "%s%d%d%s", command, &para_line, &line_num, extra);

   if(num_of_val == 3 && para_line > 0 && line_num > 0) {
      if(remove_line(doc, para_line, line_num) == FAILURE) {
	      printf("remove_line failed\n");
      }

      return SUCCESS;
   }

   return FAILURE;
}

int user_load_file(Document *doc, char *request) {
   char name[MAX_STR_SIZE + 1], file[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
   int num_of_val = sscanf(request, "%s%s%s", name, file, extra);
   
   if(num_of_val == 2) {
      if(load_file(doc, file) == FAILURE) {
	      printf("load_file failed\n");
      }

      return SUCCESS;
   }

   return FAILURE;
}

int user_replace_text(Document *doc, char *request) {
   char command[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1], replace[MAX_STR_SIZE + 1], temp1[MAX_STR_SIZE + 1], temp2[MAX_STR_SIZE + 1];   
   int num_of_val = sscanf(request, "%s%s%s", command, temp1, temp2);

   if(num_of_val == 3) {
      char *target_quote_open, *target_quote_close, *replace_quote_open, *replace_quote_close;

      target_quote_open = strstr(request, "\"");

      if(target_quote_open != NULL){
         target_quote_close = strstr(target_quote_open + 1, "\"");
         
         if(target_quote_close != NULL) {
            replace_quote_open = strstr(target_quote_close + 1, "\"");
      
            if(replace_quote_open != NULL) {
               replace_quote_close = strstr(replace_quote_open + 1, "\"");

               if(replace_quote_close != NULL) {
                  strncpy(target, target_quote_open + 1, target_quote_close - target_quote_open);
                  target[target_quote_close - target_quote_open - 1] = '\0';
                  
                  strncpy(replace, replace_quote_open + 1, replace_quote_close - replace_quote_open);
                  replace[replace_quote_close - replace_quote_open - 1] = '\0';
                                       
                  if (replace_text(doc, target, replace) == FAILURE) {
                     printf("remove_line failed\n");	 
                  }	  
                        
                  return SUCCESS;	       	      
               }
            }	 
         }
      }
   }

   return FAILURE;
}

int user_highlight_text(Document *doc, char *request) {
   char command[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1], temp[MAX_STR_SIZE + 1];
   int num_of_val = sscanf(request, "%s%s", command, temp);

   if(num_of_val == 2){
      char *open_quote, *close_quote;
      
      open_quote = strstr(request, "\"");
      
      if(open_quote != NULL){
         close_quote = strstr(open_quote + 1, "\"");

         if(close_quote != NULL){
            strncpy(target, open_quote + 1, close_quote - open_quote);
            target[close_quote - open_quote - 1] = '\0';
            highlight_text(doc, target);      
            
            return SUCCESS;
         }
      }
   }   
   return FAILURE;
}

int user_remove_text(Document *doc, char *request) {
   char command[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1], temp[MAX_STR_SIZE + 1];
   int num_of_val = sscanf(request, "%s%s", command, temp);

   if(num_of_val == 2){
      char *open_quote, *close_quote;
      
      open_quote = strstr(request, "\"");
      
      if(open_quote != NULL){
         close_quote = strstr(open_quote + 1, "\"");

         if(close_quote != NULL){
            strncpy(target, open_quote + 1, close_quote - open_quote);
            target[close_quote - open_quote - 1] = '\0';
            remove_text(doc, target);      
            
            return SUCCESS;
         }
      }
   }   
   return FAILURE;
}

int user_save_document(Document *doc, char *request) {
   char command[MAX_STR_SIZE + 1], file[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
   int num_of_val = sscanf(request, "%s%s%s", command, file, extra);

   if(num_of_val == 2) {
      if(save_document(doc, file) == FAILURE) {
	      printf("save_document failed\n");
      }      

      return SUCCESS;
   }

   return FAILURE;
}

int user_reset_document(Document *doc, char *request) {
   char command[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
   int num_of_val = sscanf(request, "%s%s", command, extra);
   
   if (num_of_val == 1) {
      reset_document(doc);
      return SUCCESS;
   }

   return FAILURE;
}

static int is_valid_request(Document *doc, char *request) {
   int is_valid = FAILURE;
   
   if(strstr(request, "add_paragraph_after") != NULL){
      if(user_add_paragraph_after(doc, request) == SUCCESS) {
	      is_valid = SUCCESS;
      }
   } 
   else if(strstr(request, "add_line_after") != NULL){
      if(user_add_line_after(doc, request)  == SUCCESS) {
	      is_valid = SUCCESS;
      }
   }
   else if(strstr(request, "print_document") != NULL){
      if(user_print_document(doc, request)  == SUCCESS) {
	      is_valid = SUCCESS;
      }
   }
   else if(strstr(request, "append_line") != NULL){
      if(user_append_line(doc, request)  == SUCCESS) {
	      is_valid = SUCCESS;
      }
   }   
   else if(strstr(request, "remove_line") != NULL){
      if(user_remove_line(doc, request)  == SUCCESS) {
	      is_valid = SUCCESS;
      }
   }
   else if(strstr(request, "load_file") != NULL){
      if(user_load_file(doc, request) == SUCCESS) {
	      is_valid = SUCCESS;
      }
   }
   else if(strstr(request, "replace_text") != NULL){
      if(user_replace_text(doc, request) == SUCCESS) {
	      is_valid = SUCCESS;
      }
   }
   else if(strstr(request, "highlight_text") != NULL){
      if(user_highlight_text(doc, request) == SUCCESS) {
	      is_valid = SUCCESS;
      }
   }
   else if(strstr(request, "remove_text") != NULL){
      if (user_remove_text(doc, request) == SUCCESS) {
	      is_valid = SUCCESS;
      }
   }
   else if(strstr(request, "save_document") != NULL) {
      if (user_save_document(doc, request) == SUCCESS) {
	      is_valid = SUCCESS;
      }
   }
   else if(strstr(request, "reset_document") != NULL){
      if(user_reset_document(doc, request) == SUCCESS) {
	      is_valid = SUCCESS;
      }
   }

   if(is_valid == FAILURE) {
      printf("Invalid Command\n");
   }

   return is_valid;
}

static char *my_strtok(char *str, const char *delimiters) {
    char *token_ptr = NULL, *token_start; 

    if (str != NULL) {
        token_ptr = str;
    } else if (token_ptr == NULL) {
        return NULL;  
    }

    token_ptr += strspn(token_ptr, delimiters);

    if (*token_ptr == '\0') {
        token_ptr = NULL;
        return NULL;
    }

    token_start = token_ptr;
    token_ptr += strcspn(token_ptr, delimiters);

    if (*token_ptr != '\0') {
        *token_ptr = '\0';
        token_ptr++;
    }

    return token_start;
}

int main(int argc, char *argv[]) {
   Document doc;
   FILE *in;
   char line[MAX_STR_SIZE + 1], comment, comment_line[MAX_STR_SIZE + 1];

   if(init_document(&doc, "main_document") == SUCCESS){
      if(argc == 1){
         in = stdin;
         printf("> ");
         fgets(line, MAX_STR_SIZE + 1, stdin);
         
         while(strcmp(line, "quit\n") && strcmp(line, "exit\n")){
            sscanf(line, " %c%s", &comment, comment_line);
            my_strtok(line, "\n");
               
            if(strcmp(line, "\n") != 0 && comment != '#') {
               is_valid_request(&doc, line);
            }
            printf("> ");
            fgets(line, MAX_STR_SIZE + 1, stdin);
         }
      } else if(argc == 2){
         in = fopen(argv[1], "r");
         
         if(in != NULL) {    
            while(fgets(line, MAX_STR_SIZE + 1, in)) {
               if(strcmp(line, "quit\n") && strcmp(line, "exit\n")) {
                  int num_of_val = sscanf(line, " %c%s", &comment, comment_line);
                  my_strtok(line, "\n");
                     
                  if (num_of_val > 0 && comment != '#') {
                     is_valid_request(&doc, line);
                  }
               }
               }
         } else{
            fprintf(stderr, "%s cannot be opened.\n", argv[1]);
            exit(EX_OSERR);
         }
      } else {
         fprintf(stderr, "Usage: user_interface\n");
         fprintf(stderr, "Usage: user_interface <filename>\n");
         exit(EX_USAGE);
      }
   } else{
      printf("Initialization failed\n");
   }

   fclose(in);
   exit(EXIT_SUCCESS);
}