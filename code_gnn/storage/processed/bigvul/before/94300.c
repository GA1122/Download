GC_API void * GC_CALL GC_memalign(size_t align, size_t lb)
{
    size_t new_lb;
    size_t offset;
    ptr_t result;

    if (align <= GRANULE_BYTES) return GC_malloc(lb);
    if (align >= HBLKSIZE/2 || lb >= HBLKSIZE/2) {
        if (align > HBLKSIZE) {
          return (*GC_get_oom_fn())(LONG_MAX-1024);  
        }
        return GC_malloc(lb <= HBLKSIZE? HBLKSIZE : lb);
             
    }
     
     
    new_lb = lb + align - 1;
    result = GC_malloc(new_lb);
    offset = (word)result % align;
    if (offset != 0) {
        offset = align - offset;
        if (!GC_all_interior_pointers) {
            if (offset >= VALID_OFFSET_SZ) return GC_malloc(HBLKSIZE);
            GC_register_displacement(offset);
        }
    }
    result = (void *) ((ptr_t)result + offset);
    GC_ASSERT((word)result % align == 0);
    return result;
}
