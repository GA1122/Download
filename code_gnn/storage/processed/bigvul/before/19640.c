GC_API void GC_CALL GC_free(void * p)
{
    struct hblk *h;
    hdr *hhdr;
    size_t sz;  
    size_t ngranules;    
    void **flh;
    int knd;
    struct obj_kind * ok;
    DCL_LOCK_STATE;

    if (p == 0) return;
         
#   ifdef LOG_ALLOCS
      GC_err_printf("GC_free(%p), GC: %lu\n", p, (unsigned long)GC_gc_no);
#   endif
    h = HBLKPTR(p);
    hhdr = HDR(h);
#   if defined(REDIRECT_MALLOC) && \
        (defined(GC_SOLARIS_THREADS) || defined(GC_LINUX_THREADS) \
         || defined(MSWIN32))
         
         
         
         
        if (0 == hhdr) return;
#   endif
    GC_ASSERT(GC_base(p) == p);
    sz = hhdr -> hb_sz;
    ngranules = BYTES_TO_GRANULES(sz);
    knd = hhdr -> hb_obj_kind;
    ok = &GC_obj_kinds[knd];
    if (EXPECT(ngranules <= MAXOBJGRANULES, TRUE)) {
        LOCK();
        GC_bytes_freed += sz;
        if (IS_UNCOLLECTABLE(knd)) GC_non_gc_bytes -= sz;
                 
                 
                 
        if (ok -> ok_init) {
            BZERO((word *)p + 1, sz-sizeof(word));
        }
        flh = &(ok -> ok_freelist[ngranules]);
        obj_link(p) = *flh;
        *flh = (ptr_t)p;
        UNLOCK();
    } else {
        size_t nblocks = OBJ_SZ_TO_BLOCKS(sz);
        LOCK();
        GC_bytes_freed += sz;
        if (IS_UNCOLLECTABLE(knd)) GC_non_gc_bytes -= sz;
        if (nblocks > 1) {
          GC_large_allocd_bytes -= nblocks * HBLKSIZE;
        }
        GC_freehblk(h);
        UNLOCK();
    }
}
