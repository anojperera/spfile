/* generic linked list class
   Fri Jul 23 09:32:47 GMTDT 2010 */

#ifndef __ALIST__
#define __ALIST__

/* c++ compaitiblity */
#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>	

    /* forward declaration */
    typedef struct _node aNode;
    
    /* node structure */    
    struct _node
    {
	unsigned int ix_counter;
	unsigned int ix;
	void* data;
	aNode* next;
	aNode* previous;
	aNode* last;		/* pointer to last node */
    };

    /* constructor */
    aNode* aList_New();

    /* add a node to list */
    void aList_Add(aNode** obj,
		   void* data,
		   unsigned int sz);

    /* add a node from back */
    /* void aList_AddB(aNode** obj, */
    /* 		    void* data, */
    /* 		    unsigned int sz); */


    /* return the count of list */
    unsigned int  aList_Count(aNode** obj);

    /* clear list */
    void aList_Clear(aNode** obj);

    /* retrieved an item from list */
    aNode* aList_Item(aNode** obj, unsigned int ix);

    /* aList Diaplay function shall scan through
       the list and call the function pointer.
       void* pointer is used for and the user is
       expected to excersie caution converting
       data types */
    int aList_Display(aNode** obj,
		      int s_flg,
		      int (*callback) (void* , unsigned int));

    /* Display2 function is simillar to display function above.
     * Fourth argument of the function shall be passed to the
     * first argument of the callback function
     * Call back function must return 0 to indicate success.
     * If any other values were returned display function shall
     * stop.
     */
    int aList_Display2(aNode** obj,
		       int s_flg,
			   int (*callback) (void*, void*, unsigned int),
		       void * c_obj);
    
    /* c++ compatibility */
#ifdef __cplusplus
}
#endif
    

#endif /* __ALIST__ */
