#include <stdio.h>
#include "cexc.h"

void handler(struct Crash* err){
    printf("Error:%s",err->mes);
}

double Div(double a, double b){
    if (b==0.0){
        struct Crash c={"Can not divide by zero\n",0};
        _Throw(&c);
    }
    return a/b;
}

int main(){
    struct CrashHandler h={handler};
    TRY(h,
        double a=Div(1.0,0.5);
        printf("res is %.2f\n",a);
    )
    return 0;
}