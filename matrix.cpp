#include "headers.h"

pair1::pair1()
{
    line = 0;
    column = 0;
}

pair1::pair1(int l_0, int c_0)
{
    line   = l_0;
    column = c_0;
}

pair1::pair1 (const pair1& a)
{
    line   = a.line;
    column = a.column;
}

pair1& pair1::operator=(const pair1& a)
{
    line   = a.line;
    column = a.column;
    return *this;
}


matrix::matrix()
{
    size  = 12;
    val   = new double[size];
    place = new pair1[size];
}

matrix::matrix(matrix *mat)
{
    quantity_users = mat->quantity_users;
    size           = mat->size;

    val            = new double[size];
    place          = new pair1[size];
    begin          = new int[quantity_users];
    end            = new int[quantity_users];
    users          = new ulli[quantity_users];

    memcpy(place, mat->place, size*sizeof(pair1));
    memcpy(begin, mat->begin, quantity_users*sizeof(int));
    memcpy(end, mat->end, quantity_users*sizeof(int));
    memcpy(users, mat->users, quantity_users*sizeof(ulli));
}

void matrix::copy_matrix(matrix *mat)
{
    int i;
    if (quantity_users != mat->quantity_users || size != mat->size)
    {
        cout << "ERROR: illegal size of matrix\n";
        cout << "IN FILE " <<  __FILE__ << " LINE " << __LINE__ << " FUNCTION " << __FUNCTION__ << endl;
        return;
    }
    for (i = 0; i < quantity_users; i++)
    {
        begin[i] = mat->begin[i];
        end[i]   = mat->end[i];
        place[i] = mat->place[i];
    }
    for (i = 0; i < size; i++)
    {
        val[i] = mat->val[i];
    }
/*
    memcpy(place, mat->place, size*sizeof(pair1));
    memcpy(begin, mat->begin, quantity_users*sizeof(int));
    memcpy(end, mat->end, quantity_users*sizeof(int));
    memcpy(users, mat->users, quantity_users*sizeof(ulli));*/
}

matrix::~matrix()
{
    delete [] val;
    delete [] place;
    if (quantity_users > 0)
    {
        delete [] begin;
        delete [] end;
        delete [] users;
    }
}

matrix::matrix(table *tbl)
{
    int k = tbl->quantity_chains;
    int j, i, l;
    size = 0;
    quantity_users = tbl->quantity_users;


    int tmp_val = 0;
    int first, last = 0;

    for (j = 0; j < k - 1; j++)
    {
        tmp_val = tbl->hash_chains[j + 1] - tbl->hash_chains[j];
        tmp_val = tmp_val * (tmp_val - 1);
        size    += tmp_val / 2;
    }
    tmp_val = tbl->size_data - tbl->hash_chains[k - 1];
    tmp_val = tmp_val * (tmp_val - 1);
    size      += tmp_val / 2;

    val     = new double[size];
    place   = new pair1[size];
    begin   = new int[quantity_users];
    end     = new int[quantity_users];
    users   = new ulli[quantity_users];

    memcpy(users, tbl->users, quantity_users*sizeof(ulli));

    int index = 0;

    for (j = 0; j < k; j++)
    {
        first = tbl->hash_chains[j];
        last  =  (j < k - 1) ? tbl->hash_chains[j + 1] : tbl->size_data;
        for (i = first; i < last; i++)
        {
            for (l = i + 1; l < last; l++)
            {
                place[index] = pair1(tbl->user(tbl->data[i].id_p),
                                     tbl->user(tbl->data[l].id_p));
                index++;
            }

        }
    }

    qsort(place, size, sizeof(pair1), compare_pair);

    int counter = 0;
    for (j = 0; j < size - 1; j++)
    {
        if (cmp(place[j], place[j + 1]) != 0)
        {
            place[counter] = place[j];
            counter++;
        }
    }
    place[counter] = place[size - 1];
    counter++;
    size = counter;

    for (i = 0; i < size; i++)
    {
        val[i] = 0.5;
    }

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

    //cout << "sorted cleaned matrix size = " << size << endl;
}

int matrix::index(int l_0, int c_0)
{
    return index(pair1(l_0, c_0));
}

int matrix::index(pair1 p)
{

    if (p.line > quantity_users - 1 || p.column > quantity_users - 1)
    {
        return -1;
    }

    return binarySearch<pair1> (place, size, p, 'p', 'p');
}

void matrix::clean(double min_val)
{
    int i, counter;

    counter = 0;

    for (i = 0; i < size; i++)
    {
        if (val[i] > min_val)
        {
            val[counter]   = val[i];
            place[counter] = place[i];
            counter++;
        }
    }

    for (i = 0; i < quantity_users; i++)
    {
        begin[i] = -1;
        end[i]   = -1;
    }

    size = counter;

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
}

void matrix::print()
{
    int j, i;
    int ind;

    cout << endl;
    for (j = 0; j < quantity_users; j++)
    {
        for (i = 0; i < quantity_users; i++)
        {
            ind = index(j, i);
            if (ind == -1)
                cout << "   " << 0 << "   ";
            else
                printf(" %.3f ", val[ind]);
        }
        cout << endl;
    }
    cout << endl;
}

void matrix::fprint(const char * matrix_out, const char *users_out)
{
    int j;
    FILE * fout = fopen(users_out, "w");

    fprintf(fout, "%d\n", quantity_users);
    for (j = 0; j < quantity_users; j++)
    {
        fprintf(fout, "%llu %d %d\n", users[j], begin[j], end[j]);
    }
    fclose(fout);

    fout = fopen(matrix_out, "w");
    fprintf(fout, "%d\n", size);
    for (j = 0; j < size; j++)
    {
        fprintf(fout, "%d %d %lf\n", place[j].line, place[j].column, val[j]);
    }
    fclose(fout);
}

matrix::matrix(const char * matrix_in, const char *users_in)
{
    int j;

    int line;
    int column;
    double tmp_val;

    FILE *fin;
    if ( (fin = fopen (matrix_in, "r")) == NULL)
    {
        cout << "ERROR: can't open file " << matrix_in << endl;
        cout << "IN FILE " <<  __FILE__ << " LINE " << __LINE__ << " FUNCTION " << __FUNCTION__ << endl;
    }
    if (fscanf(fin, "%d", &size) != 1)
    {
        cout << "ERROR: can't read size " << endl;
        cout << "IN FILE " <<  __FILE__ << " LINE " << __LINE__ << " FUNCTION " << __FUNCTION__ << endl;
    }
    if (size < 1)
    {
        cout << "ERROR: Illegal size " << endl;
        cout << "IN FILE " <<  __FILE__ << " LINE " << __LINE__ << " FUNCTION " << __FUNCTION__ << endl;
    }
    if (size > 0)
    {
        place = new pair1[size];
        val   = new double[size];
    }

    for (j = 0; j < size; j++)
    {
        if (fscanf(fin, "%d %d %lf", &line, &column, val + j) != 3)
        {
            cout << "ERROR: can't element "  << j << endl;
            cout << "IN FILE " <<  __FILE__ << " LINE " << __LINE__ << " FUNCTION " << __FUNCTION__ << endl;
        }
        else
            place[j] = pair1(line, column);
    }
    fclose (fin);

    if ((fin = fopen (users_in, "r")) == NULL)
    {
        cout << "ERROR: can't open file " << users_in << endl;
        cout << "IN FILE " <<  __FILE__ << " LINE " << __LINE__ << " FUNCTION " << __FUNCTION__ << endl;
    }

    if (fscanf(fin, "%d", &quantity_users) != 1)
    {
        cout << "ERROR: can't read quantity of users " << endl;
        cout << "IN FILE " <<  __FILE__ << " LINE " << __LINE__ << " FUNCTION " << __FUNCTION__ << endl;
    }

    if (quantity_users > 0)
    {
        users = new ulli[quantity_users];
        begin = new int[quantity_users];
        end   = new int[quantity_users];
    }

    for (j = 0; j < quantity_users; j++)
    {
        if (fscanf(fin, "%llu %d %d", users + j, begin + j, end + j) != 3)
        {
            cout << "ERROR: matrix::matrix(char*, char*): can't user "  << j << endl;
            cout << "IN FILE " <<  __FILE__ << " LINE " << __LINE__ << " FUNCTION " << __FUNCTION__ << endl;
        }
    }
    fclose (fin);
}


void print_graph_folowers(const char * file_name, table *tbl, matrix *mat)
{
    int i, j;
    FILE *fout;

    if ((fout = fopen(file_name, "w")) == NULL)
    {
        cout << "ERROR: can't open file " << file_name << endl;
        cout << "IN FILE " <<  __FILE__ << " LINE " << __LINE__ << " FUNCTION " << __FUNCTION__ << endl;
        return;
    }

    for (i = 0; i < tbl->quantity_users; i++)
    {
        fprintf(fout , "%llu   %s  %d %d\n", tbl->users[i], tbl->user_names[i].c_str(), mat->begin[i], mat->end[i]);
        for (j = mat->begin[i]; j < mat->end[i]; j++)
        {
            fprintf(fout , "%llu   %s\n", tbl->users[mat->place[j].column], tbl->user_names[mat->place[j].column].c_str());
        }
        fprintf(fout, "\n");
    }
    fclose(fout);
}

void print_graph_folowers_test(const char * file_name, table *tbl, matrix *mat)
{
    int i, j;
    FILE *fout;

    int counter = 0;
    int step = tbl->quantity_users;

    if ((fout = fopen(file_name, "w")) == NULL)
    {
        cout << "ERROR: can't open file " << file_name << endl;
        cout << "IN FILE " <<  __FILE__ << " LINE " << __LINE__ << " FUNCTION " << __FUNCTION__ << endl;
        return;
    }

    for (i = 0; i < tbl->quantity_users; i += 5)
    {
        if (mat->end[i] - mat->begin[i] > 2)
        {
            fprintf(fout , "%llu   %s\n", tbl->users[i], tbl->user_names[i].c_str());
            if (counter > 90)
                break;
            for (j = mat->begin[i]; j < mat->end[i]; j++)
            {
                fprintf(fout , "%llu   %s\n", tbl->users[mat->place[j].column], tbl->user_names[mat->place[j].column].c_str());
                counter++;
            }
            fprintf(fout, "\n");
        }
    }
    fclose(fout);
}

void print_graph_folows(const char * file_name, table *tbl, matrix *mat)
{
    int i, j;
    FILE *fout;

    if ((fout = fopen(file_name, "w")) == NULL)
    {
        cout << "ERROR: can't open file " << file_name << endl;
        cout << "IN FILE " <<  __FILE__ << " LINE " << __LINE__ << " FUNCTION " << __FUNCTION__ << endl;
        return;
    }

    for (i = 0; i < tbl->quantity_users; i++)
    {
        fprintf(fout , "%llu   %s\n", tbl->users[i], tbl->user_names[i].c_str());

        for (j = mat->begin[i]; j < mat->end[i]; j++)
        {
            fprintf(fout , "%llu   %s\n", tbl->users[mat->place[j].line], tbl->user_names[mat->place[j].line].c_str());
        }
        fprintf(fout, "\n");
    }
    fclose(fout);
}


void matrix::sort_cl()
{

    int i;

    qsort(place, size, sizeof(pair1), compare_pair_cl);

    for (i = 0; i < quantity_users; i++)
    {
        begin[i] = -1;
        end[i]   = -1;
    }

    int tmp_column = place[0].column;
    begin[tmp_column] = 0;
    for (i = 1; i < size; i++)
    {
        if (place[i].column != tmp_column)
        {
            end[tmp_column] = i;
            tmp_column = place[i].column;
            begin[tmp_column] = i;
        }
    }
    end[tmp_column] = size;
}

void matrix::sort_lc()
{
    int i;

    qsort(place, size, sizeof(pair1), compare_pair);

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
}

int max(int a, int b)
{
    return (a < b) ? b : a;
}

int table::get_length_act_m(int user)
{
    int user_len = (user < quantity_users - 1) ?
                hash_activity_m[user + 1] - hash_activity_m[user] :
                size_activity_m - hash_activity_m[user - 1];
    return user_len;
}

void matrix::clean_minimize(table *tbl)
{
    sort_cl();
    int i;
    for (i = 0; i < quantity_users; i++)
        clean_user(tbl, i);
    sort_lc();
}

void matrix::clean_user(table * tbl, int user)
{
    int i, j, k, l, tmp_user, user_length, start1, end1;
    int start = begin[user];
    int end_   = end[user];

    if (user % 15000 == 14999)
        cout << user << endl;

    ulli **array;
    ulli *user_array;

    user_length = tbl->get_length_act_m(user);
    user_array = new ulli [user_length];
    start1 = tbl->hash_activity_m[user];
    end1 = (user < tbl->quantity_users - 1) ? tbl->hash_activity_m[user + 1] : tbl->size_activity_m;
    for (k = 0, i = start1; i < end1; i++, k++)
    {
        user_array[k] = tbl->activity_m[i].id_m;
    }

    int *length;

    array = new ulli* [end_ - start];
    length = new int [end_ - start];

    if (end_ - start < 2)
    {
        delete [] array;
        delete [] length;
        delete [] user_array;
        return;
    }


    for (l = 0, i = start; i < end_; i++, l++)
    {
        tmp_user = place[i].line;
        length[l] = tbl->get_length_act_m(tmp_user);
        array[l] = new ulli [length[l]];
        start1 = tbl->hash_activity_m[tmp_user];
        end1   = (tmp_user < tbl->quantity_users - 1) ? tbl->hash_activity_m[tmp_user + 1] : tbl->size_activity_m;
        for (k = 0, j = start1; j < end1; j++, k++)
        {
            array[l][k] = tbl->activity_m[j].id_m;
        }
    }


    for (i = 0; i < end_ - start; i++)
    {
        for (j = i + 1; j < end_ - start; j++)
        {
            int intersection = count(array[i], length[i], array[j], length[j]);
            if (intersection == length[j] && intersection < length[i])
            {
                val[start + j] = 0;
            }
            else if (intersection == length[i] && intersection < length[j])
            {
                val[start + i] = 0;
            }
            else if (2 * intersection / (length[i] + length[j]) > 0.5)
            {
                if (val[start + j] > val[start + i])
                    val[start + i] = 0;
                else
                    val[start + j] = 0;
            }
        }
    }

    for (i = 0; i < end_ - start; i++)
    {
        delete [] array[i];
    }
    delete [] length;
    delete [] user_array;
    delete [] array;
}

int count(ulli * array1, int size1, ulli* array2, int size2)
{
    int i;
    int find;

    int counter = 0;
    for (i = 0; i < size1; i++)
    {
        find = binarySearch<ulli> (array2, size2, array1[i], 'l', 'l');
        if (find > -1)
            counter++;
    }

    return counter;
}

int intersect(ulli * array1, int size1, ulli* array2, int size2)
{
    int i;
    int counter = 0;
    int find;

    for (i = 0; i < size1; i++)
    {
        find = binarySearch<ulli> (array2, size2, array1[i], 'l', 'l');
        if (find > -1)
        {
            array1[counter] = array1[i];
            counter++;
        }
    }
    size1 = counter;
    return size1;
}


double check(matrix *mat1, matrix *mat2)
{
    int rez1 = 0, rez2 = 0;
    int i;
    double rez_1;
    double rez_2;

    for (i = 0; i < mat1->size; i++)
    {
        if (mat2->index(mat1->place[i].line, mat1->place[i].column) > -1)
            rez1 += 1;
    }

    rez_1 = (double)rez1 / mat1->size;

    for (i = 0; i < mat2->size; i++)
    {
        if (mat1->index(mat2->place[i].line, mat2->place[i].column) > -1)
            rez2 += 1;
    }

    rez_2 = (double)rez2 / mat2->size;

    printf("Lost: %f, Wrong: %f ", rez_2, rez_1);

    return 2*rez_1*rez_2 / (rez_1 + rez_2);
}

double cmp_matrix(matrix * mat1, matrix * mat2)
{
    int i;
    double res = 0.0;
    int i_max = 0;

    if (mat1->size != mat2->size)
    {
        cout << "ERROR: different sizes of matrix" << endl;
        cout << "IN FILE " <<  __FILE__ << " LINE " << __LINE__ << " FUNCTION " << __FUNCTION__ << endl;
        return -1.0;
    }
    for (i = 0; i < mat1->size; i++)
    {
        if (cmp(mat1->place[i], mat2->place[i]) != 0)
        {
            cout << "ERROR: different positions" << endl;
            cout << "IN FILE " <<  __FILE__ << " LINE " << __LINE__ << " FUNCTION " << __FUNCTION__ << endl;
            return -1.0;
        }
        //res += (mat1->val[i] - mat2->val[i]) * (mat1->val[i] - mat2->val[i]);
        if (fabs((mat1->val[i] - mat2->val[i])) > res)
        {
            res = fabs((mat1->val[i] - mat2->val[i]));
            i_max = i;
        }

    }
    res /= mat2->size;
    res = sqrt(res);
    //cout << mat1->place[i_max].column << " " << mat1->place[i_max].line << " " <<mat1->val[i_max] << " " << mat2->val[i_max] <<  endl;
    return res;
}
