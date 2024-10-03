/*
Krisnajit Rajeshkhanna
UID: 119769843
Directory ID: krajeshk
*/
#include <stdio.h>
#include <math.h>
#define MAX_ASSIGN 50

void print_ordered_assign(int *arr, int size);
void copy_list(int* source, int* destination, int size);
void drop_assign(int* arr, int size, int drop);
void selection_sort(int* arr, int size);
void get_SWL(int* res, const int* arr, int size, int index);
double calc_num_score(const int* scores, const int* weights, const int* lates, int point_penalty, int weight_sum, int size);
double calc_mean(const int* arr, int point_penalty, int size);
double calc_st_dev(const int* arr, int point_penalty, int size, double mean);

int main(){
    int point_penalty, num_assign_drop, num_of_assign, i, weight_sum = 0;
    char will_calc_stats;
    double numeric_score, mean, st_dev;
    int assign_list[MAX_ASSIGN*4], copy_of_assign_list[MAX_ASSIGN*4], scores[MAX_ASSIGN], weights[MAX_ASSIGN], lates[MAX_ASSIGN];

    /*Read the first line with two integers and a character*/
    if(scanf("%d %d %c", &point_penalty, &num_assign_drop, &will_calc_stats) != 3){
        /*printf("ERROR: Invalid values provided\n");*/
        return 1;
    }

    /*Read the second line with an integer*/
    if(scanf("%d", &num_of_assign) != 1){
        /*printf("ERROR: Invalid values provided\n");*/
        return 1;
    }

    if (num_of_assign > MAX_ASSIGN) {
        /*printf("ERROR: Maximum number of assignments exceeded\n");*/
        return 1;
    }

    /*Read the third line with four integers into the array*/
    for(i = 0; i < num_of_assign; i++){
        if (scanf("%d, %d, %d, %d", 
                   &assign_list[i * 4], 
                   &assign_list[i * 4 + 1], 
                   &assign_list[i * 4 + 2], 
                   &assign_list[i * 4 + 3]) != 4) {
        /*printf("ERROR: Invalid values provided\n");*/
            return 1;
        }
    }

    copy_list(assign_list, copy_of_assign_list, num_of_assign);


    /*Terminate program if weights do no add up to 100*/
    for(i = 0; i < num_of_assign*4; i+=4){
        weight_sum += assign_list[i+2];
    }
    if(weight_sum != 100){
        printf("%d ERROR: Invalid values provided\n", weight_sum);
        return 1;
    }
    
    /*Sort assignments in order of assignment number*/
    selection_sort(assign_list, num_of_assign); 
    selection_sort(copy_of_assign_list, num_of_assign); 

    /*Drop any assignments if needed*/
    drop_assign(assign_list, num_of_assign, num_assign_drop);


    /*Get each individual data list (Scores, Weights, Lates) from assignment list*/
    get_SWL(scores, assign_list, num_of_assign-num_assign_drop, 1);
    get_SWL(weights, assign_list, num_of_assign-num_assign_drop, 2);
    get_SWL(lates, assign_list, num_of_assign-num_assign_drop, 3);
    
    /*Get new weight sum after dropping any classes*/
    weight_sum = 0;
    for(i = 0; i < num_of_assign-num_assign_drop; i++){
        weight_sum += weights[i];
    }

    /*Calculations (numeric, mean, st. dev.)*/
    numeric_score = calc_num_score(scores, weights, lates, point_penalty, weight_sum, num_of_assign-num_assign_drop);
    mean = calc_mean(copy_of_assign_list, point_penalty, num_of_assign);
    st_dev = calc_st_dev(copy_of_assign_list, point_penalty, num_of_assign, mean);
    
    /*Final output*/
    printf("Numeric Score: %5.4f\n", numeric_score);
    printf("Points Penalty Per Day Late: %d\n", point_penalty);
    printf("Number of Assignments Dropped: %d\n", num_assign_drop);
    printf("Values Provided:\nAssignment, Score, Weight, Days Late\n");
    print_ordered_assign(copy_of_assign_list, num_of_assign);
    if(will_calc_stats == 'y' || will_calc_stats == 'Y')
        printf("Mean: %5.4f, Standard Deviation: %5.4f\n", mean, st_dev);
    return 0;
}

/*Copy one array into another*/
void copy_list(int* source, int* destination, int size){
    int i;
    for(i = 0; i < size*4; i++){
        destination[i] = source[i];
    }
}

/*For number of dropped assignemnts, let the set of 4 values (#, score, weight, late) = (-1,-1,-1,-1)*/
void drop_assign(int* arr, int size, int drop){
    if(drop > 0 && drop <= size){
        int i;
        for (i = 0; i < drop; i++) {
            int min_index = -1;
            int min_value = 100; 

            /*Find the index of the minimum value in the array*/
            int j, k;
            for (j = 0; j < size; j++) {
                int value = arr[4*j + 1] * arr[4*j + 2];
                if (arr[4*j] != 0 && value < min_value){
                    min_value = value;
                    min_index = j;
                }
            }

            if (min_index != -1) {
                /*Set the entire set of 4 values to -1*/
                for (k = 0; k < 4; k++) {
                    arr[4*min_index + k] = -1;
                }
            }
        }
    }
} 

/*Calculate numeric score SUM[(score - late points) * weight/weightsum] or 0 if late > score*/
double calc_num_score(const int* scores, const int* weights, const int* lates, int point_penalty, int weight_sum, int size){
    double res = 0;
    int i = 0;
    for(i = 0; i < size; i++){
        res += ((scores[i] - lates[i] * point_penalty) < 0 ? 0 : (scores[i] - lates[i] * point_penalty)) * weights[i] / ((double)weight_sum);
    }
    return res;
}

/*Calculate mean SUM[x]/n*/
double calc_mean(const int* arr, int point_penalty, int size){
    double res = 0;
    int i = 0;
    for(i = 0; i < size; i++){
        res += (arr[4*i+1] - arr[4*i+3] * point_penalty) < 0 ? 0 : (arr[4*i+1] - arr[4*i+3] * point_penalty);
    }
    return res/size;
}

/*Calculate st dev using formula SQRT[SUM(x-mean)/n]*/
double calc_st_dev(const int* arr, int point_penalty, int size, double mean){
    double res = 0;
    int i = 0;
    for(i = 0; i < size; i++){
        res += pow((arr[4*i+1] - arr[4*i+3] * point_penalty) - mean, 2);
    }
    return sqrt(res/size);
}

/*Get Scores, Weights, or Lates from AssignmentList (distinguished by last parameter which takes 1, 2, or 3)*/
void get_SWL(int* res, const int* arr, int size, int index){
    int i, j;
    for(i = index, j = 0; i < size*4; i+=4, j++){
        if(arr[i] != -1)
            res[j] = arr[i];
    }
}

/*Print Assignments (Will be ordered by this point when called for final output)*/
void print_ordered_assign(int* arr, int size){
    int i;
    for(i = 0; i < size*4; i+=4){
        printf("%d, %d, %d, %d\n", arr[i], arr[i+1], arr[i+2], arr[i+3]);
    }
}

/*Selection Sort algorithm to swap based on Assignment Number, and swap 4 values at a time till assignments are all in order*/
void selection_sort(int* arr, int size) {
    int i;
    for(i = 0; i < size - 1; i++) {
        int min_index = i;
        int j, k;
        /*Find the index of the minimum value in the array*/
        for(j = i + 1; j < size; j++) {
            if (arr[j * 4] < arr[min_index * 4]) {
                min_index = j;
            }
        }

        /*Swap elements at index i and min_index*/
        for(k = 0; k < 4; k++) {
            int temp = arr[i * 4 + k];
            arr[i * 4 + k] = arr[min_index * 4 + k];
            arr[min_index * 4 + k] = temp;
        }
    }
}