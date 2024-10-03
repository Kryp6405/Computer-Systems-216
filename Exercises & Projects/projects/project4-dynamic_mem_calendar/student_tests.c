#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}


/*Test 1: checking initializing, printing (to random file / stdout), and deleting calendar with arbitrary inputs*/
static int test1() {
   int days = 7;
   FILE *file;
   Calendar *calendar;

   file = fopen("test_1.txt", "w");
   if(file == NULL)
      file = stdout;

   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if(print_calendar(calendar, file, 1) == SUCCESS) {
         fclose(file);
         return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/*Test 2: checking adding event to every day in arbitrarily initialized calendar*/
static int test2() {
   int days = 3;
   Calendar *calendar;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        if (add_event(calendar, "day 1 event", 500, 60, NULL, 1) == SUCCESS &&
            add_event(calendar, "day 2 event", 600, 70, NULL, 2) == SUCCESS &&
            add_event(calendar, "day 3 event", 700, 80, NULL, 3) == SUCCESS) {
            return destroy_calendar(calendar);
        }
   }
   
   return FAILURE;
}

/*Test 3: checking adding event to random days in arbitrarily initialized calendar*/
static int test3() {
   int days = 4;
   Calendar *calendar;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        if (add_event(calendar, "breakfast", 500, 25, NULL, 2) == SUCCESS &&
            add_event(calendar, "brunch", 800, 40, NULL, 1) == SUCCESS &&
            add_event(calendar, "lunch", 1000, 50, NULL, 2) == SUCCESS &&
            add_event(calendar, "snack", 1400, 10, NULL, 4) == SUCCESS &&
            add_event(calendar, "dinner", 2000, 60, NULL, 2) == SUCCESS) {
            return destroy_calendar(calendar);
        }
   }
   
   return FAILURE;
}

/*Test 4: checking adding events with same name*/
static int test4(){
   int days = 4;
   Calendar *calendar;

   if(init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
      add_event(calendar, "same name event", 500, 50, NULL, 1);

      if(add_event(calendar, "same name event", 600, 50, NULL, 1) == FAILURE &&
         add_event(calendar, "same name event", 500, 60, NULL, 3) == FAILURE){
            return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/*Test 5: checking finding two different events that exist*/
static int test5(){
   Calendar *calendar;
   Event *event1, *event2;
   int days = 4;

   if(init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
      add_event(calendar, "find event 1", 500, 50, NULL, 2);
      add_event(calendar, "find event 2", 550, 55, NULL, 4);

      if(find_event(calendar, "find event 1", &event1) == SUCCESS &&
         find_event(calendar, "find event 2", &event2) == SUCCESS){
            return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/*Test 6: checking finding two different events that do not exist*/
static int test6(){
   Calendar *calendar;
   Event *event1, *event2;
   int days = 4;

   if(init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
      add_event(calendar, "find event 1", 500, 50, NULL, 2);
      add_event(calendar, "find event 2", 550, 55, NULL, 4);

      if(find_event(calendar, "find event 3", &event1) == FAILURE &&
         find_event(calendar, "find event 4", &event2) == FAILURE){
            return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/*Test 7: checking finding one event on a particular day*/
static int test7(){
   Calendar *calendar;
   Event *event1, *event2;
   int days = 4;

   if(init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
      add_event(calendar, "find event in day 2", 500, 50, NULL, 2);

      if(find_event_in_day(calendar, "find event in day 2", 2, &event1) == SUCCESS &&
         find_event_in_day(calendar, "find event in day 2", 3, &event2) == FAILURE){
            return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/*Test 8: checking removing first and last event on a particular day*/
static int test8(){
   Calendar *calendar;
   int days = 4;

   if(init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
      add_event(calendar, "first event", 500, 50, NULL, 2);
      add_event(calendar, "filler event", 650, 65, NULL, 2);
      add_event(calendar, "filler event 2", 600, 60, NULL, 2);
      add_event(calendar, "last event", 550, 55, NULL, 2);

      if(remove_event(calendar, "first event") == SUCCESS &&
         remove_event(calendar, "last event") == SUCCESS){
            return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/*Test 9: checking removing middle events on a particular day*/
static int test9(){
   Calendar *calendar;
   int days = 4;

   if(init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
      add_event(calendar, "first event", 500, 50, NULL, 2);
      add_event(calendar, "filler event", 650, 65, NULL, 2);
      add_event(calendar, "filler event 2", 600, 60, NULL, 2);
      add_event(calendar, "last event", 550, 55, NULL, 2);

      if(remove_event(calendar, "filler event") == SUCCESS &&
         remove_event(calendar, "filler event 2") == SUCCESS){
            return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/*Test 10: checking finding and removing events with no existing events*/
static int test10(){
   Calendar *calendar;
   Event *event;
   int days = 4;

   if(init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){

      if(find_event(calendar, "find event", &event) == FAILURE &&
         remove_event(calendar, "remove event") == FAILURE){
            return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/*Test 11: checking clearing all events on two particular days*/
static int test11(){
   Calendar *calendar;
   int days = 4;

   if(init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
      add_event(calendar, "first event 1", 500, 50, NULL, 2);
      add_event(calendar, "second event 1", 650, 65, NULL, 2);
      add_event(calendar, "first event 2", 600, 60, NULL, 3);
      add_event(calendar, "second event 2", 550, 55, NULL, 3);

      if(clear_day(calendar, 2) == SUCCESS &&
         clear_day(calendar, 3) == SUCCESS){
            return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/*Test 12: checking clearing calendar but keeping events array*/
static int test12(){
   Calendar *calendar;
   int days = 4;

   if(init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
      add_event(calendar, "event 1", 500, 50, NULL, 1);
      add_event(calendar, "event 2", 650, 65, NULL, 2);
      add_event(calendar, "event 3", 600, 60, NULL, 2);
      add_event(calendar, "event 4", 550, 55, NULL, 4);

      if(clear_calendar(calendar) == SUCCESS){
            return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/*Test 13: checking clearing days & calendar with no existing events*/
static int test13(){
   Calendar *calendar;
   int days = 4;

   if(init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){

      if(clear_day(calendar, 1) == SUCCESS &&
         clear_day(calendar, 3) == SUCCESS &&
         clear_calendar(calendar) == SUCCESS){
            return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   printf("------------------\nRunning Test 1...\n");
   if (test1() == FAILURE) {
      printf("Failed Test 1.\n------------------\n\n");
      result = FAILURE;
   }
   else printf("Passed Test 1!\n------------------\n\n");

   printf("------------------\nRunning Test 2...\n");
   if (test2() == FAILURE) {
      printf("Failed Test 2.\n------------------\n\n");
      result = FAILURE;
   }
   else printf("Passed Test 2!\n------------------\n\n");

   printf("------------------\nRunning Test 3...\n");
   if (test3() == FAILURE) {
      printf("Failed Test 3.\n------------------\n\n");
      result = FAILURE;
   }
   else printf("Passed Test 3!\n------------------\n\n");

   printf("------------------\nRunning Test 4...\n");
   if (test4() == FAILURE) {
      printf("Failed Test 4.\n------------------\n\n");
      result = FAILURE;
   }
   else printf("Passed Test 4!\n------------------\n\n");

   printf("------------------\nRunning Test 5...\n");
   if (test5() == FAILURE) {
      printf("Failed Test 5.\n------------------\n\n");
      result = FAILURE;
   }
   else printf("Passed Test 5!\n------------------\n\n");

   printf("------------------\nRunning Test 6...\n");
   if (test6() == FAILURE) {
      printf("Failed Test 6.\n------------------\n\n");
      result = FAILURE;
   }
   else printf("Passed Test 6!\n------------------\n\n");

   printf("------------------\nRunning Test 7...\n");
   if (test7() == FAILURE) {
      printf("Failed Test 7.\n------------------\n\n");
      result = FAILURE;
   }
   else printf("Passed Test 7!\n------------------\n\n");

   printf("------------------\nRunning Test 8...\n");
   if (test8() == FAILURE) {
      printf("Failed Test 8.\n------------------\n\n");
      result = FAILURE;
   }
   else printf("Passed Test 8!\n------------------\n\n");

   printf("------------------\nRunning Test 9...\n");
   if (test9() == FAILURE) {
      printf("Failed Test 9.\n------------------\n\n");
      result = FAILURE;
   }
   else printf("Passed Test 9!\n------------------\n\n");

   printf("------------------\nRunning Test 10...\n");
   if (test10() == FAILURE) {
      printf("Failed Test 10.\n------------------\n\n");
      result = FAILURE;
   }
   else printf("Passed Test 10!\n------------------\n\n");

   printf("------------------\nRunning Test 11...\n");
   if (test11() == FAILURE) {
      printf("Failed Test 11.\n------------------\n\n");
      result = FAILURE;
   }
   else printf("Passed Test 11!\n------------------\n\n");

   printf("------------------\nRunning Test 12...\n");
   if (test12() == FAILURE) {
      printf("Failed Test 12.\n------------------\n\n");
      result = FAILURE;
   }
   else printf("Passed Test 12!\n------------------\n\n");

   printf("------------------\nRunning Test 13...\n");
   if (test13() == FAILURE) {
      printf("Failed Test 13.\n------------------\n\n");
      result = FAILURE;
   }
   else printf("Passed Test 13!\n------------------\n\n");

   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/
   
   if (result == FAILURE) {
      printf("At Least 1 Test Was Not Successful.\n");
      exit(EXIT_FAILURE);
   }
   
   printf("All Tests Were Successful!\n");
   return EXIT_SUCCESS;
}
