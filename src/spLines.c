/* implementation of lines collection
 * Sun Jan 30 19:00:47 GMT 2011 */

#include "spLines.h"

#define SPS_LINEFEED "\n"


/* callback function construct buffer */
static int pr_gen_buff(void* obj, void* data, unsigned int ix)
{
    if(!obj || !data)
	return 1;
    
    /* splines* sps = (splines*) obj; */

    if(((splines*) obj)->var_gcnt == 0)
	{
	    strcpy(((splines*) obj)->var_tbuff,
		   spline_get_mlinetxt((spline*) data));
	}
    else
	{
	    strcat(((splines*) obj)->var_tbuff,
		   spline_get_mlinetxt((spline*) data));
	}

    strcat(((splines*) obj)->var_tbuff,
	   SPS_LINEFEED);
    ((splines*) obj)->var_gcnt++;

    /* printf ("%s\n",((spline*) data)->var_value); */
    /* printf ("%s\n",((spline*) data)->var_adrs); */
    return 0;
}

/* Constructor */
splines* splines_new()
{
    /* declare temporary object */
    splines* tmp = (splines*)
	malloc(sizeof(splines));

    /* check for NULL pointer */
    if(!tmp)
	return NULL;

    /* set elements */
    tmp->var_spline = aList_New();
    tmp->var_count = 0;
    tmp->var_tbuffsz = 0;
    tmp->var_tbuffsz = 0;
    tmp->var_flg = 0;
    tmp->var_gcnt = 0;

    return tmp;
}

/* Destructor */
void splines_delete(splines** obj)
{
    /* check for object pointer */
    if(!obj || !(*obj))
	return;

    /* declare node */
    aNode* tmp = (*obj)->var_spline;

    while(tmp != NULL)
	{
	    spline_delete((spline**) &tmp->data);

	    tmp = tmp->next;
	}

    aList_Clear(&(*obj)->var_spline);
    /* free buffer if exist */
    if ((*obj)->var_tbuff)
	{
	    free((*obj)->var_tbuff);
	    (*obj)->var_tbuff = NULL;
	}

    /* free object itself */
    free(*obj);

    *obj = NULL;
}

/* add spline to collection */
int splines_add(splines* obj,
		    const char* line)
{
    if(!obj || !line)
	return 0;

    /* create an object of type spline */
    spline* sp = spline_new();

    /* check for NULL pointer */
    if(!sp)
	return 0;

    spline_set_linetxt(sp, line);

    /* run separate */
    if(spline_run2(sp))
	{
	    obj->var_tbuffsz += spline_get_mlinesz(sp) + 1;
	    aList_Add(&obj->var_spline,
		      (void*) sp,
		      sizeof(spline));

	    obj->var_flg = 1;
	    obj->var_count++;
	}

    /* delete temporary object */
    /* spline_delete(&sp); */
    return obj->var_tbuffsz;
}


/* Get buffer text */
char* splines_get_buff(splines* obj)
{
    /* check for object pointer */
    if(!obj)
	return NULL;

    if(obj->var_flg == 0)
	return obj->var_tbuff;

    /* construct buffer */
    if(!obj->var_tbuffsz)
	return NULL;

    /* reset general counter */
    obj->var_gcnt = 0;

    obj->var_tbuff = (char*)
	malloc(sizeof(char) * (obj->var_tbuffsz + 1));
    
    /* call display2 to construct buffer */
    aList_Display2(&obj->var_spline,
		   1,
		   pr_gen_buff,
		   (void*) obj);

    /* set flag to indicate variable
     * properties were not set thus next
     * subsequent call to get buff shall return
     * internal buffer instead of reconstructing */
    obj->var_flg = 0;
    return obj->var_tbuff;
}

/* get count of number of objects */
unsigned int splines_get_count(splines* obj)
{
    if(!obj)
	return 0;
    else
	return obj->var_count;
}

/* get size of out buffer */
size_t splines_get_buffsz(splines* obj)
{
    if(!obj)
	return 0;
    else
	return obj->var_tbuffsz;
}
