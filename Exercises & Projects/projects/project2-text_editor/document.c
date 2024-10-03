/*
Krisnajit Rajeshkhanna
UID: 119769843
Directory ID: krajeshk
*/
#include <stdio.h>
#include <string.h>
#include "document.h"

int init_document(Document *doc, const char *name){
    if(doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE){
        return FAILURE;
    }

    strncpy(doc->name, name, MAX_STR_SIZE);
    doc->number_of_paragraphs = 0;
    return SUCCESS;
}

int reset_document(Document *doc){
    if(doc == NULL){
        return FAILURE;
    }

    doc->number_of_paragraphs = 0;
    return SUCCESS;
}

int print_document(Document *doc) {
    int i, j;

    if (doc == NULL) {
        return FAILURE;
    }

    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            printf("%s\n", doc->paragraphs[i].lines[j]);
            if(j == doc->paragraphs[i].number_of_lines-1 && i < doc->number_of_paragraphs-1){
                printf("\n");
            }
        }
    }

    return SUCCESS;
}

int add_paragraph_after(Document *doc, int paragraph_number) {
    int i;
    
    if (doc == NULL || paragraph_number < 0 || paragraph_number > doc->number_of_paragraphs || doc->number_of_paragraphs >= MAX_PARAGRAPHS) {
        return FAILURE;
    }

    for (i = doc->number_of_paragraphs - 1; i >= paragraph_number; i--) {
        doc->paragraphs[i + 1] = doc->paragraphs[i];
    }

    doc->paragraphs[paragraph_number].number_of_lines = 0;
    doc->number_of_paragraphs++;

    return SUCCESS;
}

int add_line_after(Document *doc, int paragraph_number, int line_number, const char *new_line) {
    int i;

    if (doc == NULL || paragraph_number <= 0 || paragraph_number > doc->number_of_paragraphs || line_number < 0 || line_number > doc->paragraphs[paragraph_number - 1].number_of_lines || new_line == NULL) {
        return FAILURE;
    }

    for (i = doc->paragraphs[paragraph_number - 1].number_of_lines - 1; i >= line_number; i--) {
        strcpy(doc->paragraphs[paragraph_number - 1].lines[i + 1], doc->paragraphs[paragraph_number - 1].lines[i]);
    }

    strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], new_line);
    doc->paragraphs[paragraph_number - 1].number_of_lines++;

    return SUCCESS;
}

int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines){
    if(doc == NULL || number_of_lines == NULL || paragraph_number <= 0 || paragraph_number > MAX_PARAGRAPHS){
        return FAILURE;
    }

    *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
    return SUCCESS;
}

int get_number_lines(Document *doc, int *number_of_lines){
    int i;

    if(doc == NULL || number_of_lines == NULL){
        return FAILURE;
    }

    *number_of_lines = 0;
    for(i = 0; i < doc->number_of_paragraphs; i++){
        *number_of_lines += doc->paragraphs[i].number_of_lines;
    }
    return SUCCESS;
}

int append_line(Document *doc, int paragraph_number, const char *new_line){
    if (doc == NULL || paragraph_number <= 0 || paragraph_number > doc->number_of_paragraphs || doc->paragraphs[paragraph_number - 1].number_of_lines >= MAX_PARAGRAPH_LINES || new_line == NULL) {
        return FAILURE;
    }

    strcpy(doc->paragraphs[paragraph_number - 1].lines[doc->paragraphs[paragraph_number - 1].number_of_lines], new_line);
    doc->paragraphs[paragraph_number - 1].number_of_lines++;
    return SUCCESS;
}

int remove_line(Document *doc, int paragraph_number, int line_number){
    int i;

    if (doc == NULL || paragraph_number <= 0 || paragraph_number > doc->number_of_paragraphs || line_number <= 0 || line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {
        return FAILURE;
    }
    
    for (i = line_number - 1; i < doc->paragraphs[paragraph_number - 1].number_of_lines - 1; i++) {
        strcpy(doc->paragraphs[paragraph_number - 1].lines[i], doc->paragraphs[paragraph_number - 1].lines[i + 1]);
    }

    doc->paragraphs[paragraph_number - 1].number_of_lines--;
    return SUCCESS;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines){
    int current_paragraph = 0, i;

    if (doc == NULL || data == NULL || data_lines == 0) {
        return FAILURE;
    }

    reset_document(doc);

    for (i = 0; i < data_lines; i++) {
        if (strlen(data[i]) == 0) {
            current_paragraph++;
            if (current_paragraph >= MAX_PARAGRAPHS) {
                return FAILURE;
            }
        } else {
            if (doc->paragraphs[current_paragraph].number_of_lines >= MAX_PARAGRAPH_LINES) {
                return FAILURE; 
            }
            strcpy(doc->paragraphs[current_paragraph].lines[doc->paragraphs[current_paragraph].number_of_lines], data[i]);
            doc->paragraphs[current_paragraph].number_of_lines++;
        }
    }

    doc->number_of_paragraphs = current_paragraph + 1;
    return SUCCESS;
}

int replace_text(Document *doc, const char *target, const char *replacement) {
    int i, j;

    if (doc == NULL || target == NULL || replacement == NULL) {
        return FAILURE;
    }

    for (i = 0; i < doc->number_of_paragraphs; ++i) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; ++j) {
            char *pos = strstr(doc->paragraphs[i].lines[j], target);

            while (pos != NULL) {
                int index = pos - doc->paragraphs[i].lines[j];
                
                if (strlen(replacement) + strlen(pos + strlen(target)) < MAX_STR_SIZE) {
                    char temp[MAX_STR_SIZE + 1];

                    strncpy(temp, doc->paragraphs[i].lines[j], index);
                    temp[index] = '\0';
                    strcat(temp, replacement);
                    strcat(temp, pos + strlen(target));
                    strcpy(doc->paragraphs[i].lines[j], temp);
                } else {
                    return FAILURE;
                }

                pos = strstr(pos + strlen(replacement), target);
            }
        }
    }

    return SUCCESS;
}

int highlight_text(Document *doc, const char *target) {
    int i, j;

    if (doc == NULL || target == NULL) {
        return FAILURE;
    }

    for (i = 0; i < doc->number_of_paragraphs; ++i) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; ++j) {
            char *pos = strstr(doc->paragraphs[i].lines[j], target);

            while (pos != NULL) {
                int index = pos - doc->paragraphs[i].lines[j];
                char highlighted_line[MAX_STR_SIZE + 1];

                strncpy(highlighted_line, doc->paragraphs[i].lines[j], index);
                highlighted_line[index] = '\0';
                strcat(highlighted_line, HIGHLIGHT_START_STR);
                strcat(highlighted_line, target);
                strcat(highlighted_line, HIGHLIGHT_END_STR);
                strcat(highlighted_line, pos + strlen(target));
                strcpy(doc->paragraphs[i].lines[j], highlighted_line);
                pos = strstr(pos + strlen(HIGHLIGHT_START_STR) + strlen(target) + strlen(HIGHLIGHT_END_STR), target);
            }
        }
    }

    return SUCCESS;
}

int remove_text(Document *doc, const char *target) {
    int i, j;

    if (doc == NULL || target == NULL) {
        return FAILURE;
    }
    
    replace_text(doc, target, "");
    return SUCCESS;
}