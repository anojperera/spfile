/* implementation of file object */

#include "spFile.h"

#define SPFILE_MAX_LINES 255

/* constructor */
spfile* spfile_new(const char* fp)
{
    if(!fp)
	return NULL;
    
    /* declare temporary variable */
    spfile* tmp = (spfile*) malloc(sizeof(spfile));

    if(!tmp)
	return NULL;

    /* set elements */
    tmp->var_sps = splines_new();

    if(!tmp->var_sps)
	{
	    free(tmp);
	    return NULL;
	}

    /* copy file name */
    size_t f_len = strlen(fp);
    tmp->var_ifilepath = (char*)
	malloc(sizeof(char) * (f_len + 1));
    strcpy(tmp->var_ifilepath, fp);

    tmp->var_ofilepath = NULL;

    return tmp;
}

/* destructor */
void spfile_delete(spfile** obj)
{
    if(!obj || !(*obj))
	return;

    /* call to delete lines collection */
    splines_delete(&(*obj)->var_sps);

    /* check file paths and delete */
    if((*obj)->var_ifilepath)
	{
	    free((*obj)->var_ifilepath);
	    (*obj)->var_ifilepath = NULL;
	}

    if ((*obj)->var_ofilepath)
	{
	    free((*obj)->var_ofilepath);
	    (*obj)->var_ofilepath = NULL;
	}

    return;
}

/* read the file */
int spfile_read(spfile* obj)
{
    if(!obj)
	return 0;

    if(!obj->var_ofilepath)
	return 0;

    /* file pointer */
    FILE* fp = fopen(obj->var_ifilepath, "r");

    /* check if file pointer was returned */
    if(!fp)
	return 0;

    /* set to begining */
    rewind(fp);
    
    char fp_line[SPFILE_MAX_LINES+1];

    int ln_cnt = 0;
    while(!feof(fp) && !ferror(fp))
	{
	    if(fgets(fp_line, SPFILE_MAX_LINES, fp))
		{
		    splines_add(obj->var_sps,
				fp_line);

		    ln_cnt++;
		}
	}

    /* close file pointer */
    fclose(fp);

    return ln_cnt;
}

/* set output path */
void spfile_set_outpath(spfile* obj,
			    const char* fp)
{
    if(!obj || !fp)
	return;

    size_t l = strlen(fp);

    if(obj->var_ofilepath)
	free(obj->var_ofilepath);

    obj->var_ofilepath = (char*)
	malloc(sizeof(char) * (l + 1));

    strcpy(obj->var_ofilepath, fp);

    return;
}

/* save file */
int spfile_exe(spfile* obj)
{
    /* check for file pointer */
    if(!obj)
	return 0;

    /* check if output file pointer was set */
    if(!obj->var_ofilepath)
	return 0;

    /* get pointer to internal buffer */
    char* ch = splines_get_buff(obj->var_sps);

    if(!ch &&
       splines_get_buffsz(obj->var_sps) == 0)
	return 0;

    /* open file for writing */
    FILE* fp = fopen(obj->var_ofilepath, "w");
    if(!fp)
	return 0;

    /* write buffer to file and close file pointer */
    fwrite(ch,
	   splines_get_buffsz(obj->var_sps),
	   sizeof(char),
	   fp);

    int rt_val = 0;
    if(!ferror(fp))
	rt_val = splines_get_buffsz(obj->var_sps);
    
    fclose(fp);

    return rt_val;
}
