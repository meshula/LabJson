//
//  LabBson.h
//  LabJson
//

#pragma once
#include "LabJson/bson.h"

namespace Lab {
    
    class Bson {
    public:
        Bson() {
            b = (bson*) malloc(sizeof(bson));
            bson_init(b);
        }
        
        ~Bson() {
            bson_destroy(b);
            free(b);
        }
        bson* b;
    };
    
}
