#include"../../globals/globals.h"
#include <stddef.h>
#include <stdlib.h>
#include"buffer.h"

static const real_t* current_location;

int buffer_init(unsigned int dimension_problem){   
    return SUCCESS;
}
int buffer_cleanup(void){
    return SUCCESS;
}

int buffer_renew(const real_t* current_location_){
    current_location=current_location_;
    return SUCCESS;
}

real_t buffer_get_current_f(){return 0;}
const real_t* buffer_get_current_location(){return current_location;}
const real_t*  buffer_get_current_df(){return NULL;}
