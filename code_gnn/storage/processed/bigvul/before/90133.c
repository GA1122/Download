static void* FUZ_mallocDebug(void* counter, size_t size)
{
    mallocCounter_t* const mcPtr = (mallocCounter_t*)counter;
    void* const ptr = malloc(size);
    if (ptr==NULL) return NULL;
    DISPLAYLEVEL(4, "allocating %u KB => effectively %u KB \n",
        (U32)(size >> 10), (U32)(malloc_size(ptr) >> 10));   
    mcPtr->totalMalloc += size;
    mcPtr->currentMalloc += size;
    if (mcPtr->currentMalloc > mcPtr->peakMalloc)
        mcPtr->peakMalloc = mcPtr->currentMalloc;
    mcPtr->nbMalloc += 1;
    return ptr;
}