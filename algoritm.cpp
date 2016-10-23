#include "headers.h"

long int get_fulltime(void)
{
struct timeval buf;
gettimeofday(&buf, 0);
return buf.tv_sec * 100 + buf.tv_usec / 10000;
}

void algorithm(table *tbl, matrix *weight, matrix* lag, int *time_A, int *time_B){

    int iter, m, j, i, k;
    int index, del;

    int tmp_time;

    unsigned long long int id_m;

    double done;
    double tmp;

    int M = tbl->quantity_chains;
    int N = tbl->size_data;
    int *hash_chains = tbl->hash_chains;
    msg * data    = tbl->data;
    msg * trace;

    int length;
    int start;
    int end;
/*
    int quantity_users = weight->quantity_users;
    int size = weight->size;
*/
    //matrix lag(weight);
    matrix numerator(weight);
    matrix denominator_weight(weight);
    matrix denominator_lag(weight);

    for (j = 0; j < weight->size; j++){
        weight->val[j]            = 0.5;
        lag->val[j]                = 0.5;
    }

    for (iter = 0; iter < MAX_ITER; iter++){
        for (j = 0; j < weight->size; j++){
            numerator.val[j]          = 0.0;
            denominator_lag.val[j]    = 0.0;
            denominator_weight.val[j] = 0.0;
        }
        if (iter < 10)
            printf("iter  %d A progress: ", iter);
        else
            printf("iter %d A progress: ", iter);

        tmp_time = clock();

        for (m = 0; m < M; m ++){
            done = (double ) m / M;
            printf("%.2f", done);

            int * users;
            start  = hash_chains[m];
            end    = (m < M - 1) ? hash_chains[m + 1] : N;
            length = end - start;
            trace  = data + start;
            id_m   = data[start].id_m;

            users = new int [length];

            for (i = 0; i < length; i++){
                users[i] = tbl->user(trace[i].id_p);
            }

            double tmp_val = 0;

            tmp_val = 0;
            for (i = 1; i < length; i++){
                tmp_val = 0;
                for (j = 0; j < i; j++){
                    tmp_val += p(tbl, weight, lag, j, i, m);
                }
                for (k = 0; k < i; k++){
                    index = numerator.index(users[k], users[i]);
                    del   = delta(tbl, k + start, i + start);
                    tmp   = p(tbl, weight, lag, k, i, m) / tmp_val;
                    if (!tmp == tmp)
                        tmp = 0;
                    numerator.val[index]          += tmp;
                    denominator_lag.val[index]    += tmp * del;
                    tmp = 1 - pow(1 - lag->val[index], del);
                    if (tmp != tmp)
                        tmp = 0;
                    denominator_weight.val[index] += tmp;
                }

            }
/*
            cout << "numerator:\n";
            numerator.print();
            cout << "denom_lag:\n";
            denominator_lag.print();
            cout << "denom_weight:\n";
            denominator_weight.print(); */
/*
            for (j = 0; j < length; j++){
                //int user = trace[j].id_p;
                int start2 = numerator.begin[users[j]];
                int end2   = numerator.end[users[j]];
                if (start2 < 0)
                    break;
                for (i = start2; i < end2; i++){
                    index = weight->index(users[j], weight->place[i].column);
                    if (index > -1 && !exist(tbl, weight->place[i].column, id_m)){
                        denominator_weight.val[index] += 1.0;
                    }
                }
            }

*/
            delete [] users;

            printf("\b\b\b\b");
        }

        tmp_time = clock() - tmp_time;
        *time_A += tmp_time;

        printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        if (iter < 10)
            printf("iter  %d B progress: ", iter);
        else
            printf("iter %d B progress: ", iter);

        int line;
        int column;

        tmp_time = clock();

        for (i = 0; i < weight->size; i++){
            done = (double ) i / weight->size;
            printf("%.2f", done);

            line   = denominator_weight.place[i].line;
            column = denominator_weight.place[i].column;

            int start = tbl->hash_activity_m[line];
            int end   = (line < tbl->quantity_users - 1) ? tbl->hash_activity_m[line + 1] : tbl->size_activity_m;

            for (j = start; j < end; j++){
                if (!exist(tbl, column, tbl->activity_m[j].id_m))
                    denominator_weight.val[i] += 1.0;
            }

            printf("\b\b\b\b");
        }
        printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");

        tmp_time = clock() - tmp_time;
        *time_B += tmp_time;

        double value;
        for (i = 0; i < weight->size; i++){
            value = (fabs(numerator.val[i]) < EPS || fabs(denominator_lag.val[i]) < EPS) ? 0 :
                             numerator.val[i] / denominator_lag.val[i];

            if (value != value)
                value = 0.0;
            if (value < EPS)
                value = 0.0;
            lag->val[i] = value;

            value = (fabs(numerator.val[i]) < EPS || fabs(denominator_weight.val[i]) < EPS) ? 0 :
                             numerator.val[i] / denominator_weight.val[i];
            if (value != value)
                value = 0.0;
            if (value < EPS)
                value = 0.0;
            weight->val[i] = value;
        }

        //cout << endl;
    }
    *time_A /= MAX_ITER;
    *time_B /= MAX_ITER;
    //cout << "time_A = " << time_A / CLOCKS_PER_SEC << endl << "time_B = " << time_B / CLOCKS_PER_SEC;
}

bool exist(table *tbl, int id, unsigned long long int id_m){

    //int id = tbl->user(id_p);

    int start = tbl->hash_activity_m[id];
    int end   = (id < tbl->quantity_users - 1) ? tbl->hash_activity_m[id + 1] : tbl->size_activity_m;
    int size = end - start;
    msg * container = tbl->activity_m + start;
    msg element;
    element.time          = 0;
    element.retweet_count = 0;
    element.id_m          = id_m;
    element.id_p          = tbl->users[id];
    int rez = binarySearch<msg>(container, size, element, 'm', 'm');

    if (rez > 0)
        return true;
    return false;
}

int active(table *tbl, msg ms){

    int id = tbl->user(ms.id_p);

    int start = tbl->hash_activity_t[id];
    int end   = (id < tbl->quantity_users - 1) ? tbl->hash_activity_t[id + 1] : tbl->size_activity_t;
    int size = end - start;
    msg * container = tbl->activity_t + start;

    int rez = binarySearch<msg>(container, size, ms, 'm', 't');

    if (rez <= 0)
        return -1;
    return container[rez].time - container[rez - 1].time;

}


int delta(table *tbl, int src, int dest){

    int act = active(tbl, tbl->data[dest]);
    int rez = (tbl->data[dest].time - tbl->data[src].time) / MIN_INTERVAL;
    if (rez == 0)
        rez ++;
    //if (act < SLEEP_INTERVAL)
    //    return rez;
    //else if (rez*MIN_INTERVAL < FAST_INTERVAL)
    //    return rez;
    return 1;
}

int get_user(table *tbl, int i, int m){
    // i - user_id in trace, m - message_id. Returns local user_id by id from twitter
    if (m > tbl->quantity_chains - 1 || m < 0){
        cout << " ERROR: get_user(): illegal msg " << m << endl;
        return -1;
    }
    return tbl->user(tbl->data[i + tbl->hash_chains[m]].id_p);
}

double p(table *tbl, matrix *weight, matrix * lag, int j, int i, int m){
    double rez = 0;
    int start = tbl->hash_chains[m];
    int del = delta(tbl, start + j, start + i);
    int l;

    int dest = get_user(tbl, j, m);
    int src  = get_user(tbl, i, m);

    int place = lag->index(dest, src);

    rez  = lag->val[place];

    rez *= pow(1 - lag->val[place], del);

    rez *= weight->val[place];

    return rez;
}

double p(table *tbl, matrix *weight, matrix * lag, int place, int j, int i, int m){
    double rez = 0;
    int start = tbl->hash_chains[m];
    int del = delta(tbl, start + j, start + i);
    int l;

    int dest = get_user(tbl, j, m);
    int src  = get_user(tbl, i, m);

    //int place = lag->index(dest, src);

    rez  = lag->val[place];

    rez *= pow(1 - lag->val[place], del);

    rez *= weight->val[place];

    return rez;
}
