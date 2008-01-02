/*************
* com_chdir.c
* $Id: com_chdir.c,v 1.4 2008/01/02 12:55:55 pnenzi Exp $
************/

#include <config.h>
#include <ngspice.h>

#include <wordlist.h>

#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#include "com_chdir.h"
#include "quote.h"
#include "streams.h"


void
com_chdir(wordlist *wl)
{
    char *s;
#ifdef HAVE_PWD_H
    struct passwd *pw;
#endif
    char localbuf[257];
    int copied = 0;

    s = NULL;

    if (wl == NULL) {

	s = getenv("HOME");

#ifdef HAVE_PWD_H
	if (s == NULL) {
	    pw = getpwuid(getuid());
	    if (pw == NULL) {
		fprintf(cp_err, "Can't get your password entry\n");
		return;
	    }           
	    s = pw->pw_dir;
	}
#endif
    } else {
        s = cp_unquote(wl->wl_word);
	copied = 1;
    }


    if (s != NULL)
       if (chdir(s) == -1)
           perror(s);

    if (copied)
	tfree(s);

#ifdef HAVE_GETCWD
    s = getcwd(localbuf, sizeof(localbuf));
    if (s)
	    printf("Current directory: %s\n", s);
    else
	    fprintf(cp_err, "Can't get current working directory.\n");
#endif

    return;

}
