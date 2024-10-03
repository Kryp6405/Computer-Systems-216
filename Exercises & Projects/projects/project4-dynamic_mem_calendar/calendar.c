#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"

int init_calendar(const char * name, int days, int (*comp)(const void *, const void *), void (*free_info)(void *), Calendar **calendar){
    if(name == NULL || days < 1 || *calendar == NULL){
        return FAILURE;
    }

    *calendar = (Calendar*)malloc(sizeof(Calendar) * days);
    if(*calendar == NULL){
        return FAILURE;
    }

    (*calendar)->name = (char*)malloc(strlen(name)+1);
    if((*calendar)->name == NULL){
        return FAILURE;
    }
    strcpy((*calendar)->name, name);

    (*calendar)->events = (Event **)calloc(sizeof(Event*), days);
    if((*calendar)->events == NULL){
        return FAILURE;
    }

    (*calendar)->total_events = 0;
    (*calendar)->days = days;
    (*calendar)->comp_func = comp;
    (*calendar)->free_info_func = free_info;

    return SUCCESS;
}

int print_calendar(Calendar *calendar, FILE *file, int is_printed){
    int i;

    if(calendar == NULL || file == NULL){
        return FAILURE;
    }

    if(is_printed){
        fprintf(file, "Calendar's Name: \"%s\"\nDays: %d\nTotal Events: %d\n\n", calendar->name, calendar->days, calendar->total_events);
    }

    fprintf(file, "**** Events ****\n");
    if(calendar->total_events > 0){
        for(i = 0; i < calendar->days; i++){
            Event *curr = calendar->events[i];
            
            fprintf(file, "Day %d\n", i + 1);
            while(curr != NULL){
                fprintf(file, "Event's Name: \"%s\", Start_time: %d, Duration: %d\n", curr->name, curr->start_time, curr->duration_minutes);
                curr = curr->next;
            }
        }
    }
    return SUCCESS;
}

int add_event(Calendar *calendar, const char *name, int start_time, int duration_minutes, void *info, int day){
    Event *curr = calendar->events[day - 1], *prev = NULL, *new_event;
    
    if(calendar == NULL || name == NULL || day < 1 || day > calendar->days || start_time < 0 || start_time > 2400 || duration_minutes <= 0){
        return FAILURE;
    }

    if(find_event(calendar, name, &new_event) == SUCCESS){
        return FAILURE;
    }

    new_event = (Event*)malloc(sizeof(Event));
    if(new_event == NULL){
        return FAILURE;
    }

    new_event->name = (char*)malloc(strlen(name) + 1);
    if(new_event->name == NULL){
        return FAILURE;
    }
    strcpy(new_event->name, name);

    new_event->start_time = start_time;
    new_event->duration_minutes = duration_minutes;
    new_event->info = info;

    while(curr != NULL){
        if(calendar->comp_func(new_event, curr) < 0){
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if(prev == NULL){
        new_event->next = calendar->events[day - 1];
        calendar->events[day - 1] = new_event;
    } else {
        prev->next = new_event;
        new_event->next = curr;
    }

    calendar->total_events++;

    return SUCCESS;
}

int find_event(Calendar *calendar, const char *name, Event **event_out){
    int i;

    if(calendar == NULL || name == NULL){
        return FAILURE;
    }

    for(i = 0; i < calendar->days; i++){
        if(find_event_in_day(calendar, name, i+1, event_out) == SUCCESS){
            return SUCCESS;
        }
    }

    return FAILURE;
}

int find_event_in_day(Calendar *calendar, const char *name, int day, Event **event_out){
    Event *curr = calendar->events[day - 1];
    
    if(calendar == NULL || name == NULL || day < 1 || day > calendar->days){
        return FAILURE;
    }

    while(curr != NULL){
        if(strcmp(curr->name, name) == 0){
            *event_out = curr;
            return SUCCESS;
        }

        curr = curr->next;
    }

    return FAILURE;
}

int remove_event(Calendar *calendar, const char *name){
    int i;

    if(calendar == NULL || name == NULL){
        return FAILURE;
    }

    for(i = 0; i < calendar->days; i++){
        Event *curr = calendar->events[i], *prev = NULL;

        while(curr != NULL){
            if(strcmp(curr->name, name) == 0){
                Event *dummy = curr;

                curr = curr->next;
                if(prev == NULL){
                    calendar->events[i] = curr;
                } else {
                    prev->next = curr;
                }
                if(calendar->free_info_func != NULL){
                    calendar->free_info_func(dummy->info);
                }
                free(dummy->name);
                free(dummy);
                
                calendar->total_events--;

                return SUCCESS;
            }

            prev = curr;
            curr = curr->next;
        }
    }

    return FAILURE;
}

void *get_event_info(Calendar *calendar, const char *name){
    Event *event;

    if (find_event(calendar, name, &event) == SUCCESS) {
        return event->info;
    }
    
    return NULL;
}

int clear_day(Calendar *calendar, int day){
    int num_events_del = 0;
    Event *curr = calendar->events[day - 1];
    
    if(calendar == NULL || day < 1 || day > calendar->days){
        return FAILURE;
    }

    while(curr != NULL){
        Event* dummy = curr;
        curr = curr->next;
        if(dummy->info != NULL && calendar->free_info_func != NULL){
            calendar->free_info_func(dummy->info);
        }
        free(dummy->name);
        free(dummy);
        num_events_del++;
    }

    calendar->total_events -= num_events_del;
    calendar->events[day - 1] = NULL;

    return SUCCESS;
}

int clear_calendar(Calendar *calendar){
    int i;
    if(calendar == NULL){
        return FAILURE;
    }

    for(i = 0; i < calendar->days; i++){
        clear_day(calendar, i + 1);
    }

    return SUCCESS;
}

int destroy_calendar(Calendar *calendar){
    if(calendar == NULL){
        return FAILURE;
    }

    clear_calendar(calendar);
    free(calendar->events);
    free(calendar->name);
    free(calendar);

    return SUCCESS;
}