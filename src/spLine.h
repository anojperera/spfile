/* Functions declared to speparate the contents of the 
 * line Fri Jan 28 17:38:48 GMT 2011 */

#ifndef _SPLINE_H_
#define _SPLINE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* define separation labels */

typedef struct _spline spline;

struct _spline
{
    char* var_line;
    char* var_mline;

    char* var_ref;
    char* var_adrs;
    /* char* var_name; */
    char* var_value;
    size_t var_line_sz;
    size_t var_mline_sz;
    size_t var_tvalue_sz;	/* true length until the end of line */
};

/* C functions */
#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and Destructor */
    spline* spline_new();
    void spline_delete(spline** obj);

    /* set line text */
    void spline_set_linetxt(spline* obj,
			    const char* var);
    char* spline_get_linetxt(spline* obj);
    
    /* get modified line text */
    char* spline_get_mlinetxt(spline* obj);

    /* operates and set elements */
    int spline_run(spline* obj);
    int spline_run2(spline* obj);

    size_t spline_get_mlinesz(spline* obj);
    
#ifdef __cplusplus
}
#endif

#endif /* _SPLINE_H_ */
