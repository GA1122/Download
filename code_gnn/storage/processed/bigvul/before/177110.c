static void* MallocWrapper(
 void *  , int32_t size, int32_t  ) {
 void *ptr = malloc(size);
 if (ptr)
        memset(ptr, 0, size);
 return ptr;
}
