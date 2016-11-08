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


#define MIN_INTERVAL 1800
#define SLEEP_INTERVAL 3600
#define FAST_INTERVAL 300
#define MAX_ITER 5
#define EPS 1e-4

typedef unsigned long long int ulli;

int cmp(void *, void *, char, char);

class matrix;

template< typename T >
int binarySearch(T* container, int size, T element, char type_flag, char func_flag)
{
    int right = size - 1;
    int left  = 0;

    //cout  << "binary_search: type = " << type_flag << " func = " << func_flag  <<" element = " << element << endl;

    if (size < 1 || cmp(&element, container + right, type_flag, func_flag) > 0
            || cmp(&element, container + left, type_flag, func_flag) < 0)
        return -1;

    //cout  << "binary_search: type = " << type_flag << " func = " << func_flag  <<" element = " << element << endl;
    //int counter = 0;
    while ((right - left) > 0) {
        //cout << counter << ' ' << left << ' ' << right << endl;
        //counter++;
        int mid = left + (right - left) / 2;

        if (cmp(&element, container + mid, type_flag, func_flag) <= 0) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }

    if (cmp(container + right, &element, type_flag, func_flag) == 0) {
        return right;
    }

    return -1;
}

class msg{
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

class table{
public:
    int size_data; // количество сообщений
    int quantity_chains; // количество цепочек
    int quantity_users; //количество пользователей
    //int quantity_msg;
    int size_activity_t; // количество сообщений в массиве активности
    int size_activity_m; // количество сообщений во втором массиве активности
    //int *length; // длинны цепочек
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
    table(char *, char *, int);
    table(matrix *, double, int , int);
    ~table();

    int read(char*);
    void print();
    void fprint(char *);
    void sort();
    void clean();
    void get_chains();
    void print_chain(int);
    void get_users();
    void fprint_users(char *);
    void print_users();
    void get_activity();
    int clean_activity();
    void print_activity_m(int );
    void print_activity_t(int );
    void print_activity_t();
    void print_activity_m();
    void fprint_activity_t(char *);
    void fprint_activity_m(char *);
    int user(ulli);
    void get_user_names(char* , char*);
    int get_length_act_m(int);
    int compare_links(int, int, int, int);

    void generate_users(int);
};

class pair1{
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
    matrix(int quantity_users, double prob, double density); // создать тестовую матрицу
    matrix(table *);
    matrix(char *, char*);
    matrix(matrix *);
    ~matrix();

    void clean(double);
    void get_indexes();
    void generate_users(int);
    int index(pair1 );
    int index(int , int);
    void print();
    void fprint(char*, char*);
    //void fscanf(char*, char*);
    void sort_cl();
    void sort_lc();
    void clean_minimize(table *);
    void clean_user(table *, int);
};


void algorithm(table *, matrix *, matrix*, int *, int *);

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

void print_graph_folowers(char *, table *, matrix *);
void print_graph_folows(char *, table *, matrix *);
void print_graph_folowers_test(char *, table *, matrix *);

int max (int , int);

int count(ulli * , int , ulli* , int );
int intersect(ulli * , int , ulli* , int);

int bernouli(double p);

double check(matrix* mat1, matrix * mat2);
