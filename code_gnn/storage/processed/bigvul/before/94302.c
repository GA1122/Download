GC_API void * GC_CALL GC_realloc(void * p, size_t lb)
{
    struct hblk * h;
    hdr * hhdr;
    size_t sz;    
    size_t orig_sz;       
    int obj_kind;

    if (p == 0) return(GC_malloc(lb));   
    h = HBLKPTR(p);
    hhdr = HDR(h);
    sz = hhdr -> hb_sz;
    obj_kind = hhdr -> hb_obj_kind;
    orig_sz = sz;

    if (sz > MAXOBJBYTES) {
         
          word descr;

          sz = (sz+HBLKSIZE-1) & (~HBLKMASK);
          hhdr -> hb_sz = sz;
          descr = GC_obj_kinds[obj_kind].ok_descriptor;
          if (GC_obj_kinds[obj_kind].ok_relocate_descr) descr += sz;
          hhdr -> hb_descr = descr;
#         ifdef MARK_BIT_PER_OBJ
            GC_ASSERT(hhdr -> hb_inv_sz == LARGE_INV_SZ);
#         else
            GC_ASSERT(hhdr -> hb_large_block &&
                      hhdr -> hb_map[ANY_INDEX] == 1);
#         endif
          if (IS_UNCOLLECTABLE(obj_kind)) GC_non_gc_bytes += (sz - orig_sz);
           
    }
    if (ADD_SLOP(lb) <= sz) {
        if (lb >= (sz >> 1)) {
#           ifdef STUBBORN_ALLOC
                if (obj_kind == STUBBORN) GC_change_stubborn(p);
#           endif
            if (orig_sz > lb) {
               
               
               
                BZERO(((ptr_t)p) + lb, orig_sz - lb);
            }
            return(p);
        } else {
             
              void * result =
                        GC_generic_or_special_malloc((word)lb, obj_kind);

              if (result == 0) return(0);
                   
                   
              BCOPY(p, result, lb);
#             ifndef IGNORE_FREE
                GC_free(p);
#             endif
              return(result);
        }
    } else {
         
          void * result =
                GC_generic_or_special_malloc((word)lb, obj_kind);

          if (result == 0) return(0);
          BCOPY(p, result, sz);
#         ifndef IGNORE_FREE
            GC_free(p);
#         endif
          return(result);
    }
}
