#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <setjmp.h>
#define __MAX_HANDLERS 20
_Thread_local size_t __current_handlers=0;
/*
A structure that describes an error
Fields:
    mes-is error text
    code-is error code
*/
struct Crash{
    char mes[64];
    size_t code;
};
/*
A structure for handling errors
Fields:
    response-a function called when error is caught
*/
struct CrashHandler{
    void(*response)(struct Crash*);
    jmp_buf __buf;
};
_Thread_local struct CrashHandler* __handlers[__MAX_HANDLERS];
/**
 * @brief Function for throwing errors
 * @param err A arror
 * @note is recommended to return function after throw because _Throw
 * wont stop programm if there's handlers
 */
void _Throw(struct Crash* err){
    if (__current_handlers>0){
        __handlers[__current_handlers-1]->response(err);
	    longjmp(__handlers[__current_handlers-1]->__buf,1);
    }
    else if (__current_handlers<=0){
        for (int i=0;i<100;i++){
            printf("%c",'\n');
        }
        char mes[]="===ERROR OCCURED===\nMESSAGE:";
        printf("%s\n%s",mes,err->mes);
    	_Exit(1);
    }
}
/**
 * @brief Starts a danger zone
 * @return 0 if succesful and -1 if failed
 */
int _AddHandler(struct CrashHandler* handler){
    if (__current_handlers<__MAX_HANDLERS){
        __handlers[__current_handlers]=handler;
        __current_handlers++;
        return 0;
    }
    else{
        return -1;
    }
}
/**
 * @brief Ends a danger zone
 * @return 0 if succesful and -1 if failed
 */
int _DecHandler(){
    if (__current_handlers>0){ 
        __current_handlers--;
        return 0;
    }
    else{
        return -1;
    }
}
/*
usage
TRY{
    _Throw(&err);
}
CATCH(err){
    //smth
}
*/
#define TRY(res) struct CrashHandler __h={res}; if(_AddHandler(&__h)==0) if(setjmp(__h.__buf)==0) for (int __i=0;__i<1;_DecHandler(),__i++)

void handle(struct Crash* err){
    printf("Err:%s",err->mes);
}
