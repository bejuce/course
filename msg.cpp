#include "headers.h"
//сообщение

msg::msg(){
    id_m = 1;
    id_p = 1;
    time = 1;
    retweet_count = 0;
}

msg::~msg(){
}

msg::msg(const msg& ms){
    id_m = ms.id_m;
    id_p = ms.id_p;
    time = ms.time;
    retweet_count = ms.retweet_count;
}

msg& msg::operator =(const msg & ms){
    id_m = ms.id_m;
    id_p = ms.id_p;
    time = ms.time;
    retweet_count = ms.retweet_count;
    return *this;
}

bool msg::read(FILE *fin){
    int lol;
    if ((fscanf(fin, "%d", &lol) != 1))
        return false;
    if ((fscanf(fin, "%llu", &id_m) != 1))
        return false;
    if ((fscanf(fin, "%llu", &id_p) != 1))
        return false;
    if ((fscanf(fin, "%d", &time) != 1))
        return false;
    if ((fscanf(fin, "%d", &retweet_count) != 1))
        return false;
    return true;
}

void msg::print(){
    printf("%llu %llu %d %d\n", id_m, id_p, time, retweet_count);
}

void msg::fprint(FILE *fout){
    fprintf(fout, "%llu %llu %d %d\n", id_m, id_p, time, retweet_count);
}
