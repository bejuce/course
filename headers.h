#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>

using namespace std;
#include <iostream>
#include <string>
#include <map>

typedef unsigned long long int ulli;

int cmp(void *, void *, char, char);

class matrix;

template< typename T >
int binarySearch(T* container, int size, T element, char type_flag, char func_flag)
{
    int right = size - 1;
    int left  = 0;

    if (size < 1
            || cmp(&element, container + right, type_flag, func_flag) > 0
            || cmp(&element, container + left, type_flag, func_flag) < 0)
        return -1;

    while ((right - left) > 0)
    {
        int mid = left + (right - left) / 2;
        if (cmp(&element, container + mid, type_flag, func_flag) <= 0)
        {
            right = mid;
        }
        else
        {
            left = mid + 1;
        }
    }

    if (cmp(container + right, &element, type_flag, func_flag) == 0)
    {
        return right;
    }

    return -1;
}

class msg
{
public:
    ulli id_m, id_p;
    int time;
    int retweet_count;


    msg (const msg&);
    msg& operator=(const msg&);
    msg();
    ~msg();

    bool read(FILE *);
    void print();
    void fprint(FILE *);
};

class table
{
public:
    int size_data; // количество сообщений
    int quantity_chains; // количество цепочек
    int quantity_users; //количество пользователей
    int size_activity_t; // количество сообщений в массиве активности
    int size_activity_m; // количество сообщений во втором массиве активности
    int *hash_chains; // адресс цепочки
    msg *data; // сообщения
    std::map<int, std::string> user_names;
    int *hash_activity_t;
    int *hash_activity_m;
    msg *activity_t; // активность пользователей отсортированная по времени
    msg *activity_m; // активность пользователей отсортированные по сообщениям
    ulli *users; // пользователи


    table();
    table(int);
    table(const char *,const char *, int);
    ~table();

    int read(const char*);
    void print();
    void fprint(const char *);
    void print_chain(int);
    void fprint_users(const char *);
    void print_users();
    void print_activity_m(int );
    void print_activity_t(int );
    void print_activity_t();
    void print_activity_m();
    void fprint_activity_t(const char *);
    void fprint_activity_m(const char *);

    void sort();
    void clean();
    void get_chains();
    void get_users();
    void get_activity();
    int clean_activity();
    int user(ulli);
    void get_user_names(const char* ,const  char*);
    int get_length_act_m(int);
    int compare_links(int, int, int, int);


    // only for test
    void generate_users(int);
    table(matrix *, double, int , int);  // for test
};

class pair1
{
public:
    int column, line; // строка столбец

    pair1();
    pair1(int, int);
    pair1 (const pair1&);
    pair1& operator=(const pair1&);
};

class matrix
{
public:
    int quantity_users, size; // количество пользователей, количество ненулевых элементов матррицы
    double *val;
    pair1  *place; // массив полей матрицы
    int *begin;    // адрес начала новой строки
    int *end;      // адрес конца строки
    ulli * users;

    matrix();
    matrix(table *);
    matrix(const char *, const char *);
    matrix(matrix *);
    ~matrix();

    void copy_matrix(matrix*);

    void print();
    void fprint(const char*, const  char*);

    void get_indexes();
    int index(pair1 );
    int index(int , int);

    void sort_cl();
    void sort_lc();

    void clean_minimize(table *);
    void clean_user(table *, int);
    void clean(double);

    // only for test
    matrix(int quantity_users, double prob, double density); // создать тестовую матрицу
    void generate_users(int);
};

double cmp_matrix(matrix *, matrix*);

void algorithm(table *, matrix *, matrix*, matrix *, matrix*, int *, int *);

int get_user(table *, int , int );

int active(table * , msg );

bool exist(table *, int , ulli );

double p(table *, matrix *, matrix * , int , int , int );

double p(table *, matrix *, matrix * , int , int , int , int );

int delta(table *, int , int );

int cmp(msg, msg);
int cmp_act_m(msg, msg);
int cmp_act_t(msg, msg);
int compare_act_m (void const * , void const * );
int compare_act_t (void const * , void const * );
int compare_msg (void const * , void const * );
int compare_users (void const * , void const * );

int compare_pair ( void const * ,  void const * );
int cmp(pair1 , pair1 );

int compare_pair_cl ( void const * ,  void const * );
int cmp_cl(pair1 , pair1 );

int cmp(void *, void *, char, char);

void print_graph_folowers(const char *, table *, matrix *);
void print_graph_folows(const char *, table *, matrix *);
void print_graph_folowers_test(const char *, table *, matrix *);

int max (int , int);

int count(ulli * , int , ulli* , int );
int intersect(ulli * , int , ulli* , int);

int bernouli(double p);

double check(matrix* mat1, matrix * mat2);
