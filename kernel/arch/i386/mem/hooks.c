/*
Implement hooks needed for liballoc to function
*/

#include <kernel/mem/liballoc.h>
#include <kernel/mem/mem.h>
#include <stddef.h>


/** This is the hook into the local system which allocates pages. It
 * accepts an integer parameter which is the number of pages
 * required.  The page size was set up in the liballoc_init function.
 *
 * \return NULL if the pages were not allocated.
 * \return A pointer to the allocated memory.
 */
void* liballoc_alloc(size_t n_pages){
    size_t free_count = get_num_free_pages();
    if(n_pages > free_count) return NULL;
    void *start = (void *)get_page();
    for(size_t i=0; i<n_pages-1; ++i) get_page();
    return start;
}


/** This frees previously allocated memory. The void* parameter passed
 * to the function is the exact same value returned from a previous
 * liballoc_alloc call.
 *
 * The integer value is the number of pages to free.
 *
 * \return 0 if the memory was successfully freed.
 */
int liballoc_free(void* start, size_t n_pages){
    void *page = start;
    for(size_t i=0; i<n_pages; ++i){
        free_page((uint32_t*)page);
        page = (char *)page + 4096;
    }
    return 0;
}

/** This function is supposed to lock the memory data structures. It
 * could be as simple as disabling interrupts or acquiring a spinlock.
 * It's up to you to decide. 
 *
 * \return 0 if the lock was acquired successfully. Anything else is
 * failure.
 */
int liballoc_lock(){
    asm("cli");
    return 0;
}

/** This function unlocks what was previously locked by the liballoc_lock
 * function.  If it disabled interrupts, it enables interrupts. If it
 * had acquired a spinlock, it releases the spinlock. etc.
 *
 * \return 0 if the lock was successfully released.
 */
int liballoc_unlock(){
    asm("sti");
    return 0;
}