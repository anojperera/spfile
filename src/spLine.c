/* implementation of line object
 * Fri Jan 28 18:05:06 GMT 2011 */

#include "spLine.h"

#define SP_FIRST_TAG "INV"
#define SP_DELIMITER 32
#define SP_CSVDELIMTER 44

#define CHR_NUM_START 48	/* ANSI 0 */
#define CHR_NUM_END 57		/* ANSI 9 */

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

/* private function */
static inline char* pr_spline_separate_string(const char* mstr,	/* main string */
				const unsigned int* cnt,	/* counter */
				char** ostr,			/* out string */
				unsigned int* ocnt);

static inline int pr_spline_cmp_mline(char** mstr,		/* main buffer */
			       size_t sz,		/* size of buffer */
			       unsigned int del_cnt,	/* delimiter count */
			       size_t lsz,		/* line size */
			       char** buff,		/* out buffer */
			       unsigned int dil);	/* delimeter */

static inline int pr_get_value(spline** obj);
static inline int pr_get_ref(spline** obj);
static inline int pr_get_adrs(spline** obj);

/* constructor */
spline* spline_new()
{
    /* temporary object */
    spline* tmp = (spline*)
	malloc(sizeof(spline));

    /* check for object pointer */
    if(!tmp)
	return NULL;

    /* set elements */
    tmp->var_line = NULL;
    tmp->var_mline = NULL;
    tmp->var_ref = NULL;
    tmp->var_adrs = NULL;
    /* tmp->var_name = NULL; */
    tmp->var_value = NULL;
    tmp->var_line_sz = 0;
    tmp->var_mline_sz = 0;
    tmp->var_tvalue_sz = 0;

    return tmp;
}

/* destructor */
void spline_delete(spline** obj)
{
    /* check for object pointer */
    if(!obj || !(*obj))
	return;

    if((*obj)->var_line)
	{
	    free((*obj)->var_line);
	    (*obj)->var_line = NULL;
	}

    if((*obj)->var_mline)
	{
	    free((*obj)->var_mline);
	    (*obj)->var_mline = NULL;
	}

    if((*obj)->var_ref)
	{
	    free((*obj)->var_ref);
	    (*obj)->var_ref = NULL;
	}

    if((*obj)->var_adrs)
    	{
    	    free((*obj)->var_adrs);
    	    (*obj)->var_adrs = NULL;
    	}

    /* if((*obj)->var_name) */
    /* 	{ */
    /* 	    free((*obj)->var_name); */
    /* 	    (*obj)->var_name = NULL; */
    /* 	} */

    if((*obj)->var_value)
	{
	    free((*obj)->var_value);
	    (*obj)->var_value = NULL;
	}

    free(*obj);
    *obj = NULL;
}

/* set line text */
void spline_set_linetxt(spline* obj,
			    const char* var)
{
    if(!obj || !var)
	return;

    if(obj->var_line)
	free(obj->var_line);

    size_t l = strlen(var);

    obj->var_line = (char*)
	malloc(sizeof(char) * (l + 1));

    strcpy(obj->var_line, var);

    obj->var_line_sz = l;

    return;
}

/* get line */
char* spline_get_linetxt(spline* obj)
{
    if(!obj)
	return NULL;
    else
	return obj->var_line;
}

/* get mline text */
char* spline_get_mlinetxt(spline* obj)
{
    if(!obj)
	return NULL;
    else
	return obj->var_mline;
}

/* process string */
int spline_run(spline* obj)
{
    /* check for object object pointer */
    if(!obj)
	return 0;

    /* check line */
    if(!obj->var_line)
	return 0;

    /* locate the number of spaces */
    /*******************************/
    unsigned int spc_num = 0;
    unsigned int i = 0;
    size_t sp_len = strlen(obj->var_line);

    for(i = 0; i < sp_len; i++)
	{
	    if(obj->var_line[i] == SP_DELIMITER)
		spc_num++;
	}
    spc_num++;

    /* create a char buffer to hold delimited text */
    char** var_buff = (char**)
	calloc(spc_num, sizeof(char*));

    /* check for NULL pointer */
    if(!var_buff)
	return 0;

    /* initialise elements to NULL */
    for(i = 0; i < spc_num; i++)
	var_buff[i] = NULL;

    unsigned int icnt = 0;		/* internal function calendar */
    
    /* call recursive function to separate texts */
    pr_spline_separate_string(obj->var_line,
			      &spc_num,
			      var_buff,
			      &icnt);

    /* copy string to local */
    int rt_val =  pr_spline_cmp_mline(var_buff,
				      spc_num,
				      spc_num,
				      obj->var_line_sz,
				      &obj->var_mline,
				      SP_CSVDELIMTER);

    pr_get_value(&obj);
    pr_get_ref(&obj);
    pr_get_adrs(&obj);

    /* clear char buffer */
    for(i = 0; i < spc_num; i++)
	{
	    if(var_buff[i])
		{
		    free(var_buff[i]);
		    var_buff[i] = NULL;
		}
	}

    free(var_buff);

    return rt_val;
    
}

/* version 2 */
int spline_run2(spline* obj)
{
    /* check for object object pointer */
    if(!obj)
	return 0;

    /* check line */
    if(!obj->var_line)
	return 0;

    /* separate values */
    if(pr_get_value(&obj) == 0 ||
       pr_get_ref(&obj) == 0 ||
       pr_get_adrs(&obj) == 0)
	return 0;

    char t_ch[2] = {',', '\0'};

    size_t r_l = 0;		/* ref length */
    size_t a_l = 0;		/* addess length */
    size_t v_l = 0;		/* value length */

    r_l = strlen(obj->var_ref);
    a_l = strlen(obj->var_adrs);
    v_l = strlen(obj->var_value);

    if(r_l == 0 || a_l == 0 || v_l == 0)
	return 0;

    /* check pointer */
    if(obj->var_mline)
	free(obj->var_mline);

    /* work out string size */
    obj->var_mline_sz = r_l + a_l + v_l + 2;
    /* add string */
    obj->var_mline = (char*)
	malloc(sizeof(char) * (obj->var_mline_sz + 1));
    strcpy(obj->var_mline, obj->var_ref);	/* reference */
    strcat(obj->var_mline, t_ch);		/* , */
    strcat(obj->var_mline, obj->var_adrs);	/* address */
    strcat(obj->var_mline, t_ch);		/* , */
    strcat(obj->var_mline, obj->var_value);	/* value */
    /* strcat(obj->var_mline, "\n"); */
    
    return obj->var_mline_sz;
}

/* gegt m line text */
size_t spline_get_mlinesz(spline* obj)
{
    if(!obj)
	return 0;
    else
	{
	    if(obj->var_mline)
		obj->var_mline_sz = strlen(obj->var_mline);
	    return obj->var_mline_sz;
	}
}

/* separate string */
char* pr_spline_separate_string(const char* mstr,		/* main string */
				const unsigned int* cnt,	/* counter */
				char** ostr,			/* out string */
				unsigned int* ocnt)
{
    if(mstr == NULL || ostr == NULL || *cnt == *ocnt)
	return NULL;


    /* find the first occurance of the delimiter */
    char* s_ptr = strchr(mstr, SP_DELIMITER);
    if(s_ptr == NULL)
	return NULL;

    /* get base length */
    size_t b_len = strlen(mstr);

    /* get modified length */
    size_t m_len = strlen(s_ptr);

    /* copy delimited string to out buffer */
    size_t n_len = b_len - m_len;
    if(n_len <= 0)
	return NULL;

    /* create out buffer */
    ostr[*ocnt] = (char*)
	malloc(sizeof(char) * (n_len	+ 1));

    if(!ostr[*ocnt])
	return NULL;
    /* copy to out buffer */
    memcpy(ostr[*ocnt], mstr, n_len);
    ostr[*ocnt][n_len + 1] = '\0';		/* add NULL pointer */

    /* increment counter */
    (*ocnt)++;
    
    if(m_len > 1)
	{
	    pr_spline_separate_string(&s_ptr[1],
				      cnt,
				      ostr,
				      ocnt);

	    return ostr[*ocnt-1];
	}
    else
	return NULL;
}

/* prepare modified string */
int pr_spline_cmp_mline(char** mstr,	/* main buffer */
			size_t sz,		/* size of buffer */
			unsigned int del_cnt,	/* delimiter count */
			size_t lsz,		/* line size */
			char** buff,		/* out buffer */
			unsigned int dil)	/* delimeter */
{
    /* check for NULL pointer */
    if(!mstr || sz <= 0)
	return 0;

    int i = 0;
    size_t bsz = 0;
    /* prepare buffer string */
    for(i = 0; i < sz; i++)
	{
	    if(mstr[i] != NULL)
		bsz += strlen(mstr[i]);
	}

    *buff = (char*) malloc(sizeof(char) *
			  (bsz + del_cnt + 1));

    /* add delimiters to string */
    int t_flg = 0;
    for(i = 0; i < sz; i++)
	{
	    if(mstr[i] != NULL)
		{
		    t_flg++;
		    if(t_flg == 1)
			strcpy(*buff, mstr[i]);
		    else
			strcat(*buff, mstr[i]);

		    strcat(*buff, (char*) &dil);
		}
	}

    return t_flg;
}

/* get value */
int pr_get_value(spline** obj)
{
    /* object pointer not required to be
     * checked */


    
    /* set temporary char pointer of line text */
    char* tmp_chr = (*obj)->var_line;
    int i = 0;

    unsigned int n_flg = 0;		/* flag to indicate
					 * numeric unit was found */
    int ed_pos = 0;			/* end position of value */
    /* look for space counting from
     * first digit */
    for(i = (*obj)->var_line_sz;
	i > -1;
	i--)
	{
	    if(tmp_chr[i] == SP_DELIMITER &&
	       n_flg == 1)
		{
		    break;
		}

	    if(tmp_chr[i] >= CHR_NUM_START &&
	       tmp_chr[i] <= CHR_NUM_END)
		{
		    if(n_flg == 0)
			ed_pos = i;
		    
		    n_flg = 1;
		}
	}

    if(i < 0)
	return 0;

    (*obj)->var_tvalue_sz = (*obj)->var_line_sz - i;
	
    size_t l = ed_pos - i;
    
    if(l <= 0)
	return 0;

    /* check if string exist */
    if((*obj)->var_value)
	free((*obj)->var_value);

    /* allocate memory */
    (*obj)->var_value = (char*)
	malloc(sizeof(char) * (l + 1));

    /* copy string */
    i++;
    int t_cnt = 0;
    while(i < ed_pos+1)
	{
	    /* copy incrementally excluding delimiter */
	    if(tmp_chr[i]!= SP_CSVDELIMTER)
		{
		    memcpy(&(*obj)->var_value[t_cnt],
			   &tmp_chr[i],
			   sizeof(char));
		    t_cnt++;
		}
	    i++;
	}
    (*obj)->var_value[t_cnt] = '\0';

    return l;
}

/* get reference */
int pr_get_ref(spline** obj)
{
    /* object pointer not required to
     * be checked */

    /* find string */
    char* tmp_ref = strstr((*obj)->var_line,
			       SP_FIRST_TAG);

    /* check for NULL pointer */
    if(!tmp_ref)
	return 0;

    size_t t_ref = (*obj)->var_line_sz -
	strlen(tmp_ref) + strlen(SP_FIRST_TAG);

    if(t_ref <= 0)
	return 0;

    /* check for pointer and allocate memory */
    if((*obj)->var_ref)
	free((*obj)->var_ref);
    
    (*obj)->var_ref = (char*)
	malloc(sizeof(char) * (t_ref + 1));

    /* copy to object */
    memcpy((*obj)->var_ref, (*obj)->var_line, t_ref);
    (*obj)->var_ref[t_ref] = '\0';

    return t_ref;
}

/* get address */
int pr_get_adrs(spline** obj)
{
    /* object pointer not test */
    if((*obj)->var_ref == NULL ||
       (*obj)->var_value == NULL)
	return 0;

    /* end position of address portion */
    size_t ed_adrs = (*obj)->var_line_sz -
	(*obj)->var_tvalue_sz;

    /* starting position of address portion */
    size_t st_adrs = strlen((*obj)->var_ref);

    if(ed_adrs <= 0 || st_adrs <= 0)
	return 0;

    /* temporary buffer */
    char* t_ch = (char*)
	malloc(sizeof(char) * (ed_adrs - st_adrs + 1));

    /* copy address portion */
    memcpy(t_ch, &(*obj)->var_line[st_adrs],
	   ed_adrs - st_adrs);
    t_ch[ed_adrs - st_adrs] = '\0';

    /* finding the leading and trailing
     * spaces */
    int s_spc = 0;
    int e_spc = 0;

    int i = 0; 		/* counter */
    /* leading space */
    for(i = 0; i < ed_adrs - st_adrs; i++)
	{
	    if(t_ch[i] != SP_DELIMITER)
		{
		    s_spc = i;
		    break;
		}
	}

    /* endign space */
    for(i = ed_adrs - st_adrs; i > -1; i--)
	{
	    if(t_ch[i] != SP_DELIMITER)
		{
		    e_spc = i;
		    break;
		}
	}

    /* replace comma with space */
    for(i = s_spc; i < e_spc+1; i++)
	{
	    if(t_ch[i] == SP_CSVDELIMTER)
		t_ch[i] = SP_DELIMITER;
	}

    /* check if address exist */
    if((*obj)->var_adrs)
	free((*obj)->var_adrs);

    (*obj)->var_adrs = (char*)
	malloc(sizeof(char) * (e_spc - s_spc + 2));

    memcpy((*obj)->var_adrs, &t_ch[s_spc],
	   e_spc - s_spc + 1);
    (*obj)->var_adrs[e_spc - s_spc] = '\0';

    /* free temporary buffer */
    free(t_ch);
    
    return e_spc - s_spc;
}
