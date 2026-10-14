#include <stdio.h>
#include <unistd.h>
#include "com.h"



void app_main(void)
{

    printf("inicio app\n");
    com_init();


    while(1){

        sleep(1);
    }

}
