#include "headers.h"

int main(void){
    int i, max_followers = 50, tmp;

    int *stats = new int[max_followers];


    int size = -1;
    table tbl("data/msg.dat", "data/metadata.dat", size);
    matrix weight("out/mat", "out/mat_users");

    cout  << endl << "weight.size = " << weight.size << " weight.quantity_users = " << weight.quantity_users << endl << endl;
    cout << "tbl.size_data = " << tbl.size_data << endl;
    cout << "tbl.quantity_chains = " << tbl.quantity_chains << endl;
    cout << "tbl.size_activity_m = " << tbl.size_activity_m << endl;
    cout << "tbl.size_activity_t = " << tbl.size_activity_t << endl;
    cout << "tbl.quantity_users = " << tbl.quantity_users << endl << endl;

    cout << "size = " << weight.size << endl;
    weight.clean(0.3);
    cout << "size = " << weight.size << endl;

    //weight.clean_minimize(&tbl);
    weight.clean(0.5);
    cout << "size = " << weight.size << endl;

    tbl.get_user_names("data/users.dat", "data/metadata.dat");

    print_graph_folowers_test("out/graph_followers_test.dat", &tbl, &weight);

    for (i = 0; i < max_followers; i++)
        stats[i] = 0;

    FILE *fout = fopen("out/stats_followers", "w");
    for (i = 0; i < weight.quantity_users; i++){
        tmp = weight.end[i] - weight.begin[i];
        if (tmp >= 0 && tmp < max_followers)
            stats[tmp] += 1;
    }
    for (i = 0; i < max_followers; i++)
        fprintf(fout, "%d %d\n", i, stats[i]);
    fclose(fout);

    print_graph_folowers("out/graph_followers.dat", &tbl, &weight);

    weight.sort_cl();

    for (i = 0; i < max_followers; i++)
        stats[i] = 0;

    fout = fopen("out/stats_follows", "w");
    for (i = 0; i < weight.quantity_users; i++)
    {
        tmp = weight.end[i] - weight.begin[i];
        if (tmp >= 0 && tmp < max_followers)
            stats[tmp] += 1;
    }
    for (i = 0; i < max_followers; i++)
        fprintf(fout, "%d %d\n", i, stats[i]);
    fclose(fout);

    print_graph_folows("out/graph_follows.dat", &tbl, &weight);

    delete [] stats;
    return 0;
}
