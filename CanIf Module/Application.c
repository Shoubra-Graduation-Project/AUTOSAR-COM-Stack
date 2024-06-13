/* 
 * File:   Application.c
 * Author: Kareem Taha Abdelfatah Mohammed
 * https://www.linkedin.com/in/kareem-taha-ba451621a/
 * Created on December 1, 2023, 5:21 PM
 */

#include "Application.h"
#include "../Common/Std_Types.h"
#define EXIT_SUCCESS 0

Std_ReturnType ret = E_OK;
void Application_initialize(void);


int main() { 
    Application_initialize(); 
    while(1){
        
    }
    return (EXIT_SUCCESS);
}

/* initialize any pin with direction & logic */
void Application_initialize(void){

}
