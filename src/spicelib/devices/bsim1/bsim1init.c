#include <config.h>

#include <devdefs.h>

#include "bsim1itf.h"
#include "bsim1ext.h"
#include "bsim1init.h"


SPICEdev B1info = {
    {
        "BSIM1",
        "Berkeley Short Channel IGFET Model",

        &B1nSize,
        &B1nSize,
        B1names,

        &B1pTSize,
        B1pTable,

        &B1mPTSize,
        B1mPTable,
	DEV_DEFAULT
    },

    DEVparam      : B1param,
    DEVmodParam   : B1mParam,
    DEVload       : B1load,
    DEVsetup      : B1setup,
    DEVunsetup    : B1unsetup,
    DEVpzSetup    : B1setup,
    DEVtemperature: B1temp,
    DEVtrunc      : B1trunc,
    DEVfindBranch : NULL,
    DEVacLoad     : B1acLoad,
    DEVaccept     : NULL,
    DEVdestroy    : B1destroy,
    DEVmodDelete  : B1mDelete,
    DEVdelete     : B1delete, 
    DEVsetic      : B1getic,
    DEVask        : B1ask,
    DEVmodAsk     : B1mAsk,
    DEVpzLoad     : B1pzLoad,
    DEVconvTest   : B1convTest,
    DEVsenSetup   : NULL,
    DEVsenLoad    : NULL,
    DEVsenUpdate  : NULL,
    DEVsenAcLoad  : NULL,
    DEVsenPrint   : NULL,
    DEVsenTrunc   : NULL,
    DEVdisto      : B1disto,
    DEVnoise      : NULL,	/* NOISE */
                    
    DEVinstSize   : &B1iSize,
    DEVmodSize    : &B1mSize

};


SPICEdev *
get_bsim1_info(void)
{
    return &B1info;
}