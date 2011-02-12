/* Lines collection
 * Sun Jan 30 18:51:27 GMT 2011 */

#ifndef _SPLINES_H_
#define _SPLINES_H_

/* temporary label defined to override
 * static linking of archive */
#define _WIN_OVERWRITE_
#include "spLine.h"

#ifdef _WIN_OVERWRITE_
#include "alist.h"
#else
#include "../../g_list/src/alist.h"
#endif

typedef struct _splines splines;

struct _splines
{
    aNode* var_spline;			/* lines collection */
    unsigned int var_count;
    size_t var_tbuffsz;			/* buffer size */
    char* var_tbuff;			/* total buffer */
    unsigned int var_flg;		/* flag to indicate variable
					   was set */
    unsigned int var_gcnt;		/* generic counter */
};

/* C functions */

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and Destructor */
    splines* splines_new();
    void splines_delete(splines** obj);

    /* add line to collection */
    int splines_add(splines* obj,
		    const char* line);

    char* splines_get_buff(splines* obj);

    unsigned int splines_get_count(splines* obj);
    size_t splines_get_buffsz(splines* obj);


#ifdef __cplusplus
}
#endif

#endif /* _SPLINES_H_ */

