#include <config.h>

#include <devdefs.h>

#include "hfet2itf.h"
#include "hfet2ext.h"
#include "hfet2init.h"


SPICEdev HFET2info = {
    {
        "HFET2",
        "HFET2 Model",

        &HFET2nSize,
        &HFET2nSize,
        HFET2names,

        &HFET2pTSize,
        HFET2pTable,

        &HFET2mPTSize,
        HFET2mPTable,
	DEV_DEFAULT
    },

    DEVparam      : HFET2param,
    DEVmodParam   : HFET2mParam,
    DEVload       : HFET2load,
    DEVsetup      : HFET2setup,
    DEVunsetup    : HFET2unsetup,
    DEVpzSetup    : HFET2setup,
    DEVtemperature: HFET2temp,
    DEVtrunc      : HFET2trunc,
    DEVfindBranch : NULL,
    DEVacLoad     : HFET2acLoad,
    DEVaccept     : NULL,
    DEVdestroy    : HFET2destroy,
    DEVmodDelete  : HFET2mDelete,
    DEVdelete     : HFET2delete,
    DEVsetic      : HFET2getic,
    DEVask        : HFET2ask,
    DEVmodAsk     : HFET2mAsk,
    DEVpzLoad     : NULL,
    DEVconvTest   : NULL,
    DEVsenSetup   : NULL,
    DEVsenLoad    : NULL,
    DEVsenUpdate  : NULL,
    DEVsenAcLoad  : NULL,
    DEVsenPrint   : NULL,
    DEVsenTrunc   : NULL,
    DEVdisto      : NULL,
    DEVnoise      : NULL,
                    
    DEVinstSize   : &HFET2iSize,
    DEVmodSize    : &HFET2mSize

};


SPICEdev *
get_hfet2_info(void)
{
    return &HFET2info;
}