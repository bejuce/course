#include "headers.h"

long int get_fulltime(void)
{
    struct timeval buf;
    gettimeofday(&buf, 0);
    return buf.tv_sec * 100 + buf.tv_usec / 10000;
}

void algorithm(table *tbl, matrix *weight, matrix* lag, matrix *weight_prev, matrix* lag_prev, int *time_A, int *time_B)
{

    int iter, m, j, i, k;
    int index, del;
    double norm = 1;

    int tmp_time;

    ulli id_m;

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

    matrix numerator(weight);
    matrix denominator(weight);

    tbl->fprint("out_table.txt");

    for (j = 0; j < weight->size; j++)
    {
        weight->val[j]      = 0.5;
        lag->val[j]         = 0.1;
        weight_prev->val[j] = 0.5;
        lag_prev->val[j]    = 0.1;

    }
    iter = 0;
    while (norm > EPS_NORM && iter < MAX_ITER)
    {
        for (j = 0; j < weight->size; j++)
        {
            numerator.val[j]   = 0.0;
            denominator.val[j] = 0.0;
        }
        if (iter < 10)
            printf("iter %d A1 progress: ", iter);
        else
            printf("iter %d A1 progress: ", iter);

        tmp_time = clock();

        for (m = 0; m < M; m ++)
        {
            done = (double ) m / M;
            printf("%.2f", done);

            int * users;
            start  = hash_chains[m];
            end    = (m < M - 1) ? hash_chains[m + 1] : N;
            length = end - start;
            trace  = data + start;
            id_m   = data[start].id_m;

            users = new int [length];

            for (i = 0; i < length; i++)
            {
                users[i] = tbl->user(trace[i].id_p);
            }

            double tmp_val = 0;

            tmp_val = 0;
            for (i = 1; i < length; i++)
            {
                tmp_val = 0;
                for (j = 0; j < i; j++)
                    tmp_val += p(tbl, weight, lag, j, i, m);

                for (k = 0; k < i; k++)
                {
                    index = numerator.index(users[k], users[i]);
                    del   = delta(tbl, k + start, i + start);
                    tmp   = p(tbl, weight, lag, k, i, m) / tmp_val;
                    if (!tmp == tmp)
                        tmp = 0;
                    numerator.val[index]   += tmp;
                    denominator.val[index] += tmp * del;
                }

            }

            delete [] users;

            printf("\b\b\b\b");
        }
        printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        double value;
        for (i = 0; i < weight->size; i++)
        {
            value = (fabs(numerator.val[i]) < EPS || fabs(denominator.val[i]) < EPS) ? 0 :
                             numerator.val[i] / denominator.val[i];

            if (value != value)
                value = 0.0;
            if (value < EPS)
                value = 0.0;
            //value  = 0.1;
            lag->val[i] = value;
        }

        for (j = 0; j < weight->size; j++)
        {
            numerator.val[j]   = 0.0;
            denominator.val[j] = 0.0;
        }

        if (iter < 10)
            printf("iter %d A2 progress: ", iter);
        else
            printf("iter %d A2 progress: ", iter);

        for (m = 0; m < M; m ++)
        {
            done = (double ) m / M;
            printf("%.2f", done);

            int * users;
            start  = hash_chains[m];
            end    = (m < M - 1) ? hash_chains[m + 1] : N;
            length = end - start;
            trace  = data + start;
            id_m   = data[start].id_m;

            users = new int [length];

            for (i = 0; i < length; i++)
            {
                users[i] = tbl->user(trace[i].id_p);
            }

            double tmp_val = 0;

            tmp_val = 0;
            for (i = 1; i < length; i++)
            {
                tmp_val = 0;
                for (j = 0; j < i; j++)
                    tmp_val += p(tbl, weight, lag, j, i, m);

                for (k = 0; k < i; k++)
                {
                    index = numerator.index(users[k], users[i]);
                    del   = delta(tbl, k + start, i + start);
                    tmp   = p(tbl, weight, lag, k, i, m) / tmp_val;
                    if (!tmp == tmp)
                        tmp = 0;
                    numerator.val[index]      += tmp;
                    tmp = 1 - pow(1 - lag->val[index], del);
                    if (tmp != tmp)
                        tmp = 0;
                    denominator.val[index] += tmp;
                }

            }

            delete [] users;

            printf("\b\b\b\b");
        }

        printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        if (iter < 10)
            printf("iter  %d B progress: ", iter);
        else
            printf("iter %d B progress: ", iter);

        int line;
        int column;

        tmp_time = clock();

        for (i = 0; i < weight->size; i++)
        {
            done = (double ) i / weight->size;
            printf("%.2f", done);

            line   = denominator.place[i].line;
            column = denominator.place[i].column;

            int start = tbl->hash_activity_m[line];
            int end   = (line < tbl->quantity_users - 1) ? tbl->hash_activity_m[line + 1] : tbl->size_activity_m;

            for (j = start; j < end; j++)
            {
                if (!exist(tbl, column, tbl->activity_m[j].id_m))
                    denominator.val[i] += 1.0;
            }

            printf("\b\b\b\b");
        }
        //printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        printf("\n%f\n", norm);

        tmp_time = clock() - tmp_time;
        *time_B += tmp_time;

        for (i = 0; i < weight->size; i++)
        {
            value = (fabs(numerator.val[i]) < EPS || fabs(denominator.val[i]) < EPS) ? 0 :
                             numerator.val[i] / denominator.val[i];
            if (value != value)
                value = 0.0;
            if (value < EPS)
                value = 0.0;
            weight->val[i] = value;
        }

        iter++;
        norm  = 0;
        value = cmp_matrix(weight, weight_prev);
        norm  += value;

        value = cmp_matrix(lag, lag_prev);
        norm += value;

        lag_prev->copy_matrix(lag);
        weight_prev->copy_matrix(weight);
        //weight->print();
        //weight_prev->print();
    }
    cout << "norm = " << norm << " iter = " << iter << endl;
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

int active(table *tbl, msg ms)
{

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


int delta(table *tbl, int src, int dest)
{

    int act = active(tbl, tbl->data[dest]);
    int rez = (tbl->data[dest].time - tbl->data[src].time) / MIN_INTERVAL;
    if (rez == 0)
        rez ++;
    //if (act < SLEEP_INTERVAL)
    //    return rez;
    //else if (rez*MIN_INTERVAL < FAST_INTERVAL)
    //    return rez;
    rez = tbl->data[dest].time - tbl->data[src].time;
    return rez + 1;
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
