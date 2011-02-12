/* File class object to read and write files 
 * Fri Jan 28 17:27:40 GMT 2011 */

#ifndef _SPFILE_H_
#define _SPFILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "spLines.h"

/* forward declaration */
typedef struct _spfile spfile;

struct _spfile
{
    splines* var_sps;			/* lines collection */
    char* var_ifilepath;		/* input file name */
    char* var_ofilepath;		/* output file name */

};

/* C functions */

#ifdef __cplusplus
extern "C" {
#endif
    /* constructor and destructor */
    spfile* spfile_new(const char* fp);
    void spfile_delete(spfile** obj);

    int spfile_read(spfile* obj);
    void spfile_set_outpath(spfile* obj,
			    const char* fp);
    int spfile_exe(spfile* obj);
#ifdef __cplusplus
}
#endif


#endif /* _SPFILE_H_ */
