#include "headers.h"

int bernouli (double p)
{
    if ((double)rand()/RAND_MAX < p)
        return 1;
    else
        return 0;
}

matrix::matrix(int quantity_users_, double p, double density)
{
    int i, j;
    int counter = 0;
    quantity_users = quantity_users_;
    size = (int)(quantity_users*quantity_users)*density*3;

    cout << "size = " << size << endl;
    generate_users(quantity_users);

    if (size > 0)
    {
        place = new pair1[size];
        val   = new double[size];
    }

    val     = new double[size];
    place   = new pair1[size];
    begin   = new int[quantity_users];
    end     = new int[quantity_users];

    for (i = 0; i < quantity_users; i++)
    {
        for (j = 0; j < quantity_users; j++)
        {
            if (bernouli(density) == 1 && j != i)
            {
                val[counter]   = p;
                place[counter] = pair1(i, j);
                counter++;
            }
        }
    }

    size = counter;
    cout << "size = " << size << endl;
    for (i = 0; i < quantity_users; i++)
    {
        begin[i] = -1;
        end[i]   = -1;
    }

    int tmp_line = place[0].line;
    begin[tmp_line] = 0;
    for (i = 1; i < size; i++)
    {
        if (place[i].line != tmp_line)
        {
            end[tmp_line] = i;
            tmp_line = place[i].line;
            begin[tmp_line] = i;
        }
    }
    end[tmp_line] = size;

    for (i = 0; i < quantity_users; i++)
    {
        if (begin[i] == -1)
        {
            val[counter]   = p;
            place[counter] = pair1(i, rand() % quantity_users_);
            counter++;

            val[counter]   = p;
            place[counter] = pair1(i, rand() % quantity_users_);
            counter++;
        }
    }

    size = counter;

    for (i = 0; i < quantity_users; i++)
    {
        begin[i] = -1;
        end[i]   = -1;
    }

    tmp_line = place[0].line;
    begin[tmp_line] = 0;
    for (i = 1; i < size; i++)
    {
        if (place[i].line != tmp_line)
        {
            end[tmp_line] = i;
            tmp_line = place[i].line;
            begin[tmp_line] = i;
        }
    }
    end[tmp_line] = size;
}

void table::generate_users(int N)
{
    int j;
    int counter = 0;

    quantity_users = N;

    users = new ulli[quantity_users];

    for (j = 0; j < quantity_users; j++)
        users[j] = j;
}

void matrix::generate_users(int N)
{
    int j;
    int counter = 0;

    quantity_users = N;

    users = new ulli[quantity_users];

    for (j = 0; j < quantity_users; j++)
        users[j] = j;
}

table::table(matrix * mat, double read_p, int number_of_msg, int t_max)
{
    int root, local_root;
    int k, j, i, t;
    int counter, retweet_counter, retweet_counter_tmp = 0;

    quantity_chains = 0;
    size_activity_m = 0;
    size_activity_t = 0;
    quantity_users = mat->quantity_users;
    size_data = 50*number_of_msg;

    generate_users(quantity_users);

    msg ms;

    double p = mat->val[0];

    int *truemap, *roots;

    truemap  = new int[mat->quantity_users];   // вершины ещё не видели сообщение
    roots    = new int[mat->quantity_users];   // вершины которые запостили сообщение

    data = new msg[size_data];

    counter = 0;
    for (k = 0; k < number_of_msg; k++)
    {
        retweet_counter = 0;
        for (i = 0; i < mat->quantity_users; i++)
        {
            truemap[i] = 1;
            roots[i]   = -1;
        }

        root = k % mat->quantity_users;

        truemap[root]             = 0;
        roots[retweet_counter]    = root;

        ms.id_m          = k;
        ms.id_p          = root;
        ms.time          = 0;
        ms.retweet_count = retweet_counter;
        data[counter]    = ms;
        retweet_counter++;
        counter++;
        for (t = 1; t < t_max; t++)
        {
            retweet_counter     += retweet_counter_tmp;
            retweet_counter_tmp = 0;
            for (j = 0; j < retweet_counter; j++)
            {
                local_root = roots[j];
                if (local_root > -1)
                {
                    if (local_root > quantity_users - 1 || local_root < 0)
                        printf("Error\ntable::table(matrix * mat, double read_p, int number_of_msg, int t_max)\nillegal local_root %d\n", local_root);
                    for (i = mat->begin[local_root]; i < mat->end[local_root]; i++)
                    {
                        if (truemap[mat->place[i].column] == 1)
                        {
                            if(bernouli(read_p) == 1)
                            {
                                truemap[mat->place[i].column] = 0;
                                if (bernouli(p) == 1)
                                {
                                    ms.id_m          = k;
                                    ms.id_p          = mat->place[i].column;
                                    ms.time          = t;
                                    ms.retweet_count = retweet_counter + retweet_counter_tmp;
                                    data[counter]    = ms;
                                    counter++;
                                    roots[retweet_counter + retweet_counter_tmp] = mat->place[i].column;
                                    retweet_counter_tmp++;
                                }
                            }
                        }

                    }
                }
            }
        }

    }
    size_data = counter;

    delete [] truemap;
    delete [] roots;

}

int main(void)  // generating test data
{
    srand(SRAND_PARAMETER_MATRIX);
    matrix mat(QUANTITY_USERS, COPY_PROB, DENSITY);
    mat.fprint("mat/mat", "mat/users");

    srand(SRAND_PARAMETER_TABLE);
    table tbl(&mat, READ_PROB, NUM_OF_MSG, T_MAX);

    cout << "size_data = " << tbl.size_data << endl;

    tbl.fprint((char*)"data/msg.dat");
    tbl.fprint_users((char*)"data/users.dat");

    if (mat.quantity_users < 21)
        mat.print();

    FILE *fin;
    fin = fopen ("data/metadata.dat", "w");
    fprintf(fin, "%d %d", tbl.size_data, tbl.quantity_users);
    fclose(fin);

    return 0;
}
