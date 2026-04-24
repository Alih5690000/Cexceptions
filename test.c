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
    TRY(handler){
        double res=Div(5.0,2.0);
        printf("Result:%f\n",res);
    }
    TRY(handler){
        double res=Div(5.0,0.0);
        printf("Result:%f\n",res);
    }
    return 0;
}