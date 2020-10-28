#ifndef _LIBALLOC_H_
#define _LIBALLOC_H_



/*
 * If we are told to not define our own size_t, then we skip the define. 
 */
#ifndef _ALLOC_SKIP_DEFINE

#ifndef _HAVE_SIZE_T
#define _HAVE_SIZE_T

#include <stddef.h>

#endif


#ifndef	NULL
#define NULL		0
#endif

#endif

#ifdef __cplusplus
extern "C"
{
#endif


/** This is a boundary tag which is prepended to the
 * page or section of a page which we have allocated. It is
 * used to identify valid memory blocks that the
 * application is trying to free.
 */
    struct boundary_tag
    {
        unsigned int magic;     /* < It's a kind of ... */
        unsigned int size;      /* < Requested size. */
        unsigned int real_size; /* < Actual size. */
        int index;

        struct boundary_tag *split_left;
        struct boundary_tag *split_right;

        struct boundary_tag *next;
        struct boundary_tag *prev;
    };



    void *liballoc_malloc(size_t);
    void *liballoc_realloc(void *, size_t);
    void *liballoc_calloc(size_t, size_t);
    void liballoc_free(void *);


#ifdef __cplusplus
}
#endif

#endif
