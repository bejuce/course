#include "headers.h"

int main(int argc, char** argv){
    long int time = clock();
    int size = -1;
    int time_A, time_B;

    if (argc == 2){
        size = atoi(argv[1]);
    }
    table tbl((char*)"data/msg.dat", (char*)"data/metadata.dat", size);
    matrix weight(&tbl);
    matrix lag(&weight);
    //weight.fprint((char*)"out/mat", (char*)"out/mat_users");

    cout  << endl << "weight.size = " << weight.size << " weight.quantity_users = " << weight.quantity_users << endl << endl;
    cout << "tbl.size_data = " << tbl.size_data << endl;
    cout << "tbl.quantity_chains = " << tbl.quantity_chains << endl;
    cout << "tbl.size_activity_m = " << tbl.size_activity_m << endl;
    cout << "tbl.size_activity_t = " << tbl.size_activity_t << endl;
    cout << "tbl.quantity_users = " << tbl.quantity_users << endl;

    //matrix weight2((char*)"out/mat", (char*)"out/mat_users");
    //cout << "weight2.size = " << weight2.size << " weight2.quantity_users = " << weight2.quantity_users << endl << endl;
    //weight2.print();

    time = clock() - time;
    cout << "\n" << "Время " << (double)time / CLOCKS_PER_SEC << endl;
    time = clock();

    algorithm(&tbl, &weight, &lag, &time_A, &time_B);

    time = clock() - time;

    weight.fprint((char*)"out/mat", (char*)"out/mat_users");

    if (weight.quantity_users < 15){
        tbl.print_users();
        cout << endl << "weight\n";
        weight.print();
        cout << "lag\n";
        lag.print();
    }

    cout << "\ntime_A = " << (double)time_A / CLOCKS_PER_SEC << " time_B = " << (double)time_B / CLOCKS_PER_SEC << endl;
    cout << "Время " << (double)time / CLOCKS_PER_SEC << endl;
/*
    for (i = 0; i < max_folowers; i++)
        stats[i] = 0;


    FILE *fout = fopen("out/stats_folowers", "w");
    for (i = 0; i < weight.quantity_users; i++){
        tmp = weight.end[i] - weight.begin[i];
        if (tmp >= 0 && tmp < max_folowers)
            stats[tmp] += 1;
    }
    for (i = 0; i < max_folowers; i++)
        fprintf(fout, "%d %d\n", i, stats[i]);
    fclose(fout);
*/
    tbl.get_user_names((char*)"data/users.dat", (char *)"data/metadata.dat");
    print_graph_folowers((char*)"out/graph_followers.dat", &tbl, &weight);
/*
    weight.sort_cl();

    for (i = 0; i < max_folowers; i++)
        stats[i] = 0;

    fout = fopen("out/stats_folows", "w");
    for (i = 0; i < weight.quantity_users; i++){
        tmp = weight.end[i] - weight.begin[i];
        if (tmp >= 0 && tmp < max_folowers)
            stats[tmp] += 1;
    }
    for (i = 0; i < max_folowers; i++)
        fprintf(fout, "%d %d\n", i, stats[i]);
    fclose(fout);

    print_graph_folows((char*)"out/graph_folows.dat", &tbl, &weight);
*/
    return 0;
}



