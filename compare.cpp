#include "headers.h"

int cmp_users(ulli a, ulli b){
    if ( a < b){
        return -1;
    }
    else if (a > b)
        return 1;
    return 0;
}

int compare_users (void const * a, void const * b){
    return cmp_users(*(ulli*)a, *(ulli*)b);
}

int cmp(msg a, msg b){
    if (a.id_m < b.id_m)
        return -1;
    else if (a.id_m > b.id_m)
        return 1;
    else if (a.time < b.time)
        return -1;
    else if (a.time > b.time)
        return 1;
    else if (a.retweet_count < b.retweet_count)
        return -1;
    else if (a.retweet_count > b.retweet_count)
        return 1;
    return 0;
}

int compare_msg ( void const * a,  void const * b)
{
   return cmp(*(msg*)a, *(msg*)b);
}

int cmp_act_t(msg a, msg b){
    if (a.id_p < b.id_p)
        return -1;
    else if (a.id_p > b.id_p)
        return 1;
    else if (a.time < b.time)
        return -1;
    else if (a.time > b.time)
        return 1;
    return 0;
}

int compare_act_t ( void const * a,  void const * b)
{
  return cmp_act_t(*(msg*)a, *(msg*)b);
}

int cmp_act_m(msg a, msg b){
    if (a.id_p < b.id_p)
        return -1;
    else if (a.id_p > b.id_p)
        return 1;
    else if (a.id_m < b.id_m)
        return -1;
    else if (a.id_m > b.id_m)
        return 1;
    return 0;
}

int compare_act_m ( void const * a,  void const * b)
{
   return cmp_act_m(*(msg*)a, *(msg*)b);
}

int cmp(pair1 a, pair1 b){
    if (a.line < b.line)
        return -1;
    else if (a.line > b.line)
        return 1;
    else if (a.column < b.column)
        return -1;
    else if (a.column > b.column)
        return 1;
    return 0;
}

int cmp_cl(pair1 a , pair1 b){
    if (a.column < b.column)
        return -1;
    else if (a.column > b.column)
        return 1;
    else if (a.line < b.line)
        return -1;
    else if (a.line > b.line)
        return 1;
    return 0;
}

int compare_pair_cl (void const * a, void const * b){
    return cmp_cl(*(pair1*)a, *(pair1*)b);
}

int compare_pair ( void const * a,  void const * b)
{
  return cmp(*(pair1*)a, *(pair1*)b);
}

int cmp(void *a, void *b, char type_flag, char func_flag){
    // a, b have type int
    if (type_flag == 'i')
       return *(int*)a - *(int*)b;

    // a, b have type unsigned long long int
    else if (type_flag == 'l'){
        if (*(ulli*)a < *(ulli*)b)
            return -1;
        else if (*(ulli*)a > *(ulli*)b)
            return 1;
        return 0;
    }

    // a, b have type pair1
    else if (type_flag == 'p')
        return cmp(*(pair1*)a, *(pair1*)b);

    // a, b have type msg
    else if (type_flag = 'm'){
        if (func_flag == 'd') // for data array
            return cmp(*(msg*)a, *(msg*)b);
        else if (func_flag == 't') // for activity_t array
            return cmp_act_t(*(msg*)a, *(msg*)b);
        else if (func_flag == 'm') // for activity_m array
            return cmp_act_m(*(msg*)a, *(msg*)b);
    }
    cout << "ERROR: cmp error in types\n";
    return 0;
}
