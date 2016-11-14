#include "headers.h"

int main(int argc, char** argv)
{
    long int time = clock();
    int size = -1;
    int time_A, time_B;

    if (argc == 2)
    {
        size = atoi(argv[1]);
    }
    table tbl("data/msg.dat", "data/metadata.dat", size);
    matrix weight(&tbl);
    matrix lag(&weight);
    matrix weight_prev(&weight);
    matrix lag_prev(&weight);

    cout << endl << "weight.size = " << weight.size << " weight.quantity_users = " << weight.quantity_users << endl << endl;
    cout << "tbl.size_data = " << tbl.size_data << endl;
    cout << "tbl.quantity_users = " << tbl.quantity_users << endl;

    time = clock() - time;
    cout << "\n" << "Время " << (double)time / CLOCKS_PER_SEC << endl;
    time = clock();

    algorithm(&tbl, &weight, &lag, &weight_prev, &lag_prev, &time_A, &time_B);

    time = clock() - time;

    weight.fprint("out/mat", "out/mat_users");

    cout << "\ntime_A = " << (double)time_A / CLOCKS_PER_SEC << " time_B = " << (double)time_B / CLOCKS_PER_SEC << endl;
    cout << "Время " << (double)time / CLOCKS_PER_SEC << endl;

    //for (i = 0; i < max_folowers; i++)
    //    stats[i] = 0;
/*
    FILE *fout = fopen("out/stats_folowers", "w");
    for (i = 0; i < weight.quantity_users; i++)
    {
        tmp = weight.end[i] - weight.begin[i];
        if (tmp >= 0 && tmp < max_folowers)
            stats[tmp] += 1;
    }
    for (i = 0; i < max_folowers; i++)
        fprintf(fout, "%d %d\n", i, stats[i]);
    fclose(fout);

    tbl.get_user_names((char*)"data/users.dat", (char *)"data/metadata.dat");
    print_graph_folowers((char*)"out/graph_followers.dat", &tbl, &weight);

*/
    matrix mat2("mat/mat", "mat/users");

    weight.clean(0.05);
    double res;
    res = check(&weight, &mat2);
    printf("\n F1 = %f\n", res);
    if (weight.quantity_users < 21)
    {
        mat2.print();
        weight.print();
    }

    weight.sort_cl();
/*
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



