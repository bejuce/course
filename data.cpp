#include "headers.h"

table::table(){
    size_data = 10;
    data = new msg[size_data];
}

table::table(int N_0){
    size_data = N_0;
    quantity_chains = 0;
    data = new msg[size_data];
}

table::~table(){
    delete [] data;
    if (quantity_chains > 0){
        delete [] hash_chains;
    }
    if (quantity_users > 0){
        delete [] users;
    }

    if (size_activity_t > 0 && quantity_users > 0){
        delete [] activity_t;
        delete [] hash_activity_t;
    }
    if (size_activity_m > 0 && quantity_users > 0){
        delete [] activity_m;
        delete [] hash_activity_m;
    }
}

table::table(char *file_name, char* file_name2, int size){

    FILE *fin;
    if (size < 1){
        if ( (fin = fopen(file_name2, "r")) == NULL){
            size_data = 2500;
        }
        else if ((fscanf(fin, "%d", &size_data))!= 1){
            size_data = 2500;
        }
        fclose (fin);
    }
    else
        size_data = size;
    cout << "size_data =  " << size_data << endl;
    data = new msg [size_data];

    read(file_name);

    fprint((char *)"out/first.dat");

    sort();

    int i;
    int counter = 0;
    for (i = 0; i < size_data - 1; i++){
        if (data[i].id_m != data[i + 1].id_m || data[i].time != data[i + 1].time
                || data[i].id_p != data[i].id_m){
            data[counter] = data[i];
            counter++;
        }
    }
    data[counter] = data[i];
    counter++;
    size_data = counter;

    get_activity();

    fprint((char *)"out/chains.dat");

    clean();
    //cout << "cleaned\n";
    //cout << "size_data =  " << size_data << endl;
    fprint((char *)"out/chains_cleaned.dat");

    get_chains();
    get_users();

    fprint_users((char *)"out/users");

    clean_activity();
}

int table::read(char *file_name){
    FILE *fin;
    int j;
    if ((fin = fopen(file_name, "r")) == NULL){
        printf("Reading error: can't open file  %s\n", file_name);
        return -1;
    }
    for(j = 0; j < size_data; j++){
        if (!data[j].read(fin)){
            printf("Reading error: can't read msg %d\n", j);
            fclose(fin);
            return -2;
        }
    }
    fclose(fin);
    return 0;
}

void table::print(){
    int j;
    int N_0 = size_data;
    if (100 <  size_data)
        N_0 = 100;
    for(j = 0; j < N_0; j++){
        printf("%d ", j);
        data[j].print();
    }
}

void table::fprint(char* file_name){
    int j;
    FILE *fout = fopen(file_name, "w");
    for(j = 0; j < size_data; j++){
        fprintf(fout, "%d ", j);
        data[j].fprint(fout);
    }
    fclose(fout);
}

void table::clean(){
    int i;
    int counter = 0;

    ulli tmp_id;

    counter = 0;

    if (data[0].id_m == data[1].id_m){
        counter += 1;
    }
    for (i = 1; i < size_data - 1; i++){
        if (data[i].id_m == data[i - 1].id_m || data[i].id_m == data[i + 1].id_m){
            data[counter] = data[i];
            counter++;
        }
    }
    if (data[size_data - 1].id_m == data[size_data - 2].id_m){
        data[counter] = data[size_data - 1];
        counter++;
    }

    size_data = counter;

    tmp_id  = 0;
    counter = 0;

    int counter2 = 0;

    if (data[0].retweet_count == 0){
        counter2++;
        counter++;
        tmp_id = data[0].id_m;
        //cout << "lol\n";
    }
    for (i = 1; i < size_data; i++){
        if (data[i].retweet_count == 0 && data[i].id_m != tmp_id){
            counter2++;
            data[counter] = data[i];
            counter++;
            tmp_id = data[i].id_m;
        }
        else if (data[i].id_m == tmp_id){
            data[counter] = data[i];
            counter++;
        }
    }

    size_data = counter;
    quantity_chains = counter2;
}

void table::sort(){
    qsort(data, size_data, sizeof(msg), compare_msg);
}

void table::get_chains(){
    int j, counter = 1;
    if (quantity_chains <= 0)
        return;
    ulli tmp_id = 0;
    hash_chains = new int[quantity_chains];
    hash_chains[0] = 0;
    tmp_id = data[0].id_m;
    for (j = 1; j < size_data; j++){
        if (data[j].id_m != tmp_id){
            hash_chains[counter] = j;
            counter++;
            tmp_id = data[j].id_m;
        }
    }
    if (quantity_chains != counter){
        cout << "ERROR: table::get_chains quantity_chains = " << quantity_chains << " counter = " << counter << endl;
    }
}

void table::print_chain(int k){
    int j;

    if (k > quantity_chains - 1 || k < 0){
        cout << "ERROR: table::print_chaon(): No chain " << k << endl;
        return;
    }
    int start = hash_chains[k];
    int end   = (k == quantity_chains - 1) ? size_data : hash_chains[k + 1];
    for (j = start; j < end; j++)
        data[j].print();
}

void table::get_users(){
    int j;
    int counter = 0;

    quantity_users = size_data;

    users = new ulli[quantity_users];

    for (j = 0; j < size_data; j++)
        users[j] = data[j].id_p;

    qsort(users, quantity_users, sizeof(ulli), compare_users);

    for (j = 0; j < quantity_users - 1; j++){
        if (users[j] != users[j + 1]){
            users[counter] = users[j];
            counter++;
        }
    }

    users[counter] = users[quantity_users - 1];
    counter++;
    quantity_users = counter;

    //cout << "sorted quantity of users = " << quantity_users << endl;
}

void table::print_users(){
    int i;
    cout << endl;
    for (i = 0; i < quantity_users; i++)
        cout << i << ' ' << users[i] << endl;
    cout << endl;
}

int table::user(ulli user_id){
    return binarySearch<ulli>(users, quantity_users, user_id, 'l', 'l');
}

void table::get_activity(){
    int i;
    int counter = 0;

    size_activity_t = size_data;
    size_activity_m = size_data;

    activity_t = new msg[size_activity_t];
    activity_m = new msg[size_activity_m];

    for (i = 0; i < size_data; i++){
        activity_t[i] = data[i];
        activity_m[i] = data[i];
    }

    qsort(activity_m, size_activity_m, sizeof(msg), compare_act_m);
    qsort(activity_t, size_activity_t, sizeof(msg), compare_act_t);
}

int table::clean_activity(){
    int i;
    unsigned long long int tmp_id; // изначальный
    int id;                        // в программе
    int counter;

    if (size_activity_m < 1 || size_activity_t < 1 || quantity_users < 1){
        cout << "table::clean_activity(): looool\n";
        return -1;
    }

    hash_activity_m = new int [quantity_users];
    hash_activity_t = new int [quantity_users];

    // clean acticity_t
    tmp_id = activity_t[0].id_p;
    id = user(tmp_id);
    counter = 0;
    for (i = 0; i < size_activity_t; i++){
        if (tmp_id != activity_t[i].id_p){
            tmp_id = activity_t[i].id_p;
            id = user(tmp_id);
        }
        if (id > -1){
            activity_t[counter] = activity_t[i];
            counter++;
        }
    }

    size_activity_t = counter;

    counter = 1;
    hash_activity_t[0] = 0;
    tmp_id = activity_t[0].id_p;
    for (i = 1; i < size_activity_t; i++){
        if (tmp_id != activity_t[i].id_p){
            hash_activity_t[counter] = i;
            counter++;
            tmp_id = activity_t[i].id_p;
        }
    }

    fprint_activity_t((char*)"out/activity_t");

    if (counter != quantity_users){
        cout << "ERROR: table::clean_activity():" << "activity_t counter = " << counter;
        cout << " quantity of users = " << quantity_users <<  endl;
        return -1;
    }


    //clean_activity_m

    tmp_id = activity_m[0].id_p;
    id = user(tmp_id);

    counter = 0;
    for (i = 0; i < size_activity_m; i++){
        if (tmp_id != activity_m[i].id_p){
            tmp_id = activity_m[i].id_p;
            id = user(tmp_id);
        }
        if (id > -1){
            activity_m[counter] = activity_m[i];
            counter++;
        }
    }

    size_activity_m = counter;

    counter = 1;
    hash_activity_m[0] = 0;
    tmp_id = activity_m[0].id_p;
    for (i = 1; i < size_activity_m; i++){
        if (tmp_id != activity_m[i].id_p){
            hash_activity_m[counter] = i;
            counter++;
            tmp_id = activity_m[i].id_p;
        }
    }

    fprint_activity_m((char*)"out/activity_m");

    if (counter != quantity_users){
        cout << "ERROR: table::clean_activity():" << "activity_m counter = " << counter;
        cout << "quantity of users = " << quantity_users <<  endl;
        return -1;
    }
    return 0;
}

void table::print_activity_t(int k){
    int i;

    if (k > quantity_users || k < 0){
        cout << "ERROR: table::print_activity_t: illegal user: " << k << endl;
        return;
    }

    cout << "user's "  << users[k] << " activity_t:\n";



    int counter = 0;
    int start = hash_activity_t[k];
    int end = (k < quantity_users - 1) ? hash_activity_t[k + 1] : size_activity_t;

    for (i = start; i < end; i++){
        cout << activity_t[i].time << ' ';
        counter++;
        if (counter % 5 == 0)
            cout <<  endl;
    }
    cout << endl;
}

void table::print_activity_m(int k){
    int i;

    if (k > quantity_users || k < 0){
        cout << "ERROR: table::print_activity_m: illegal user: " << k << endl;
        return;
    }


    cout << "user's "  << users[k] << " activity_m:\n";

    int counter = 0;
    int start = hash_activity_m[k];
    int end = (k < quantity_users - 1) ? hash_activity_m[k + 1] : size_activity_m;

    for (i = start; i < end; i++){
        cout << activity_m[i].id_m << ' ';
        counter++;
        if (counter % 3 == 0)
            cout <<  endl;
    }
    cout << endl;
}

void table::print_activity_m(){
    int i;

    int end = (size_activity_m < 100) ? size_activity_m : 100;

    cout << endl << "activity_m:\n";
    for (i = 0; i < end; i++){
        activity_m[i].print();
    }
    cout << endl;
}

void table::print_activity_t(){
    int i;

    int end = (size_activity_t < 100) ? size_activity_t : 100;

    cout << endl << "activity_t: \n";
    for (i = 0; i < end; i++){
        activity_t[i].print();
    }
    cout << endl;

}

void table::fprint_activity_m(char * file_name){
    int i, j;
    int start;
    int end;

    FILE *fout = fopen (file_name, "w");

    for (i = 0; i < quantity_users; i++){
        fprintf(fout, "%d %llu\n", i, users[i]);
        start = hash_activity_m[i];
        end   = (i < quantity_users - 1) ? hash_activity_m[i + 1] : size_activity_m;
        for (j = start; j < end; j++)
            activity_m[j].fprint(fout);
        fprintf(fout, "\n");
    }
    fclose (fout);
}

void table::fprint_activity_t(char * file_name){
    int i, j;
    int start;
    int end;

    FILE *fout = fopen (file_name, "w");

    for (i = 0; i < quantity_users; i++){
        fprintf(fout, "%d %llu\n", i, users[i]);
        start = hash_activity_t[i];
        end   = (i < quantity_users - 1) ? hash_activity_t[i + 1] : size_activity_t;
        for (j = start; j < end; j++)
            activity_t[j].fprint(fout);
        fprintf(fout, "\n");
    }
    fclose(fout);
}


void table::fprint_users(char * file_name){
    int j;

    FILE *fout = fopen(file_name, "w");

    for (j = 0; j < quantity_users; j++){
        fprintf(fout , "%d %llu\n", j, users[j]);
    }


    fclose(fout);
}

void table::get_user_names(char * file_name, char * file_name2){
    FILE * fin;
    int i, k, int_id;

    if ((fin = fopen(file_name2, "r")) == NULL){
        cout << "ERROR: table::get_user_names: can't open file " << file_name << endl;
        return;
    }

    int N;
    if ((fscanf(fin, "%d", &N)) != 1){
        cout << "ERROR: table::get_user_names: can't read N/n";
        return;
    }

    fclose(fin);

    char * buff;
    buff = new char [1024];
    string str;
    ulli id;

    if ((fin = fopen(file_name, "r")) == NULL){
        cout << "ERROR: table::get_user_names: can't open file " << file_name << endl;
        return;
    }
/*
    while (fscanf(fin, "%llu %s", &id, str) == 2){
        user_names.insert ( pair<int, string>(user(id),str) );
    }*/

    for (i = 0; i < N; i++){
        if ((fscanf(fin, "%llu", &id)) != 1){
            cout << "ERROR: table::get_user_names: can't read id " << i << endl;
            return;
        }
        /*if ((fscanf(fin, "%s", buff)) != 1){
            cout << "ERROR: table::get_user_names: can't read name " << i << endl;
            return;
        }*/
        char c;
        k = 0;
        while((fscanf(fin, "%c", &c)) == 1){
            if (c != '\n'){
                buff[k] = c;
                k++;
            }
            else
                break;
        }
        str.assign(buff, k);
        //cout << "get name " << str << endl;
        int_id = user(id);
        if (int_id > -1)
            user_names.insert ( pair<int, string>(int_id,str) );
    }

    cout << "\nsize user_names " << user_names.size() << endl << endl;
    fclose(fin);
    //delete buff;
}
