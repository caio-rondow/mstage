#include "omega.h"

int main(){

    Omega net(256,4,1);

    net.route(3,4);
    net.route(3,15);

    net.display();


    return 0;
}