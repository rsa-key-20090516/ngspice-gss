/**********
Copyright 1999 Regents of the University of California.  All rights reserved.
Author: 1998 Samuel Fung, Dennis Sinitsky and Stephen Tang
File: b3soiddtrunc.c          98/5/01
**********/


#include "ngspice.h"
#include <stdio.h>
#include <math.h>
#include "cktdefs.h"
#include "b3soidddef.h"
#include "sperror.h"
#include "suffix.h"


int
B3SOIDDtrunc(inModel,ckt,timeStep)
GENmodel *inModel;
register CKTcircuit *ckt;
double *timeStep;
{
register B3SOIDDmodel *model = (B3SOIDDmodel*)inModel;
register B3SOIDDinstance *here;

#ifdef STEPDEBUG
    double debugtemp;
#endif /* STEPDEBUG */

    for (; model != NULL; model = model->B3SOIDDnextModel)
    {    for (here = model->B3SOIDDinstances; here != NULL;
	      here = here->B3SOIDDnextInstance)
	 {
#ifdef STEPDEBUG
            debugtemp = *timeStep;
#endif /* STEPDEBUG */
            CKTterr(here->B3SOIDDqb,ckt,timeStep);
            CKTterr(here->B3SOIDDqg,ckt,timeStep);
            CKTterr(here->B3SOIDDqd,ckt,timeStep);
#ifdef STEPDEBUG
            if(debugtemp != *timeStep)
	    {  printf("device %s reduces step from %g to %g\n",
                       here->B3SOIDDname,debugtemp,*timeStep);
            }
#endif /* STEPDEBUG */
        }
    }
    return(OK);
}


