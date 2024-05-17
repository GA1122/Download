GC_API void GC_CALL GC_generic_malloc_many(size_t lb, int k, void **result)
{
    void *op;
    void *p;
    void **opp;
    size_t lw;       
    size_t lg;       
    signed_word my_bytes_allocd = 0;
    struct obj_kind * ok = &(GC_obj_kinds[k]);
    DCL_LOCK_STATE;

    GC_ASSERT(lb != 0 && (lb & (GRANULE_BYTES-1)) == 0);
    if (!SMALL_OBJ(lb)) {
        op = GC_generic_malloc(lb, k);
        if (EXPECT(0 != op, TRUE))
            obj_link(op) = 0;
        *result = op;
        return;
    }
    lw = BYTES_TO_WORDS(lb);
    lg = BYTES_TO_GRANULES(lb);
    if (EXPECT(GC_have_errors, FALSE))
      GC_print_all_errors();
    GC_INVOKE_FINALIZERS();
    LOCK();
    if (!EXPECT(GC_is_initialized, TRUE)) GC_init();
     
      if (GC_incremental && !GC_dont_gc) {
        ENTER_GC();
        GC_collect_a_little_inner(1);
        EXIT_GC();
      }
     
     
    {
        struct hblk ** rlh = ok -> ok_reclaim_list;
        struct hblk * hbp;
        hdr * hhdr;

        rlh += lg;
        while ((hbp = *rlh) != 0) {
            hhdr = HDR(hbp);
            *rlh = hhdr -> hb_next;
            GC_ASSERT(hhdr -> hb_sz == lb);
            hhdr -> hb_last_reclaimed = (unsigned short) GC_gc_no;
#           ifdef PARALLEL_MARK
              if (GC_parallel) {
                  signed_word my_bytes_allocd_tmp =
                                (signed_word)AO_load(&GC_bytes_allocd_tmp);
                  GC_ASSERT(my_bytes_allocd_tmp >= 0);
                   
                   
                   

                  if (my_bytes_allocd_tmp != 0) {
                    (void)AO_fetch_and_add(&GC_bytes_allocd_tmp,
                                           (AO_t)(-my_bytes_allocd_tmp));
                    GC_bytes_allocd += my_bytes_allocd_tmp;
                  }
                  GC_acquire_mark_lock();
                  ++ GC_fl_builder_count;
                  UNLOCK();
                  GC_release_mark_lock();
              }
#           endif
            op = GC_reclaim_generic(hbp, hhdr, lb,
                                    ok -> ok_init, 0, &my_bytes_allocd);
            if (op != 0) {
               
               
               
               
              GC_bytes_found += my_bytes_allocd;
#             ifdef PARALLEL_MARK
                if (GC_parallel) {
                  *result = op;
                  (void)AO_fetch_and_add(&GC_bytes_allocd_tmp,
                                         (AO_t)my_bytes_allocd);
                  GC_acquire_mark_lock();
                  -- GC_fl_builder_count;
                  if (GC_fl_builder_count == 0) GC_notify_all_builder();
                  GC_release_mark_lock();
                  (void) GC_clear_stack(0);
                  return;
                }
#             endif
              GC_bytes_allocd += my_bytes_allocd;
              goto out;
            }
#           ifdef PARALLEL_MARK
              if (GC_parallel) {
                GC_acquire_mark_lock();
                -- GC_fl_builder_count;
                if (GC_fl_builder_count == 0) GC_notify_all_builder();
                GC_release_mark_lock();
                LOCK();
                 
                 
                 
              }
#           endif
        }
    }
     
     
     
      opp = &(GC_obj_kinds[k].ok_freelist[lg]);
      if ( (op = *opp) != 0 ) {
        *opp = 0;
        my_bytes_allocd = 0;
        for (p = op; p != 0; p = obj_link(p)) {
          my_bytes_allocd += lb;
          if ((word)my_bytes_allocd >= HBLKSIZE) {
            *opp = obj_link(p);
            obj_link(p) = 0;
            break;
          }
        }
        GC_bytes_allocd += my_bytes_allocd;
        goto out;
      }
     
    {
        struct hblk *h = GC_allochblk(lb, k, 0);
        if (h != 0) {
          if (IS_UNCOLLECTABLE(k)) GC_set_hdr_marks(HDR(h));
          GC_bytes_allocd += HBLKSIZE - HBLKSIZE % lb;
#         ifdef PARALLEL_MARK
            if (GC_parallel) {
              GC_acquire_mark_lock();
              ++ GC_fl_builder_count;
              UNLOCK();
              GC_release_mark_lock();

              op = GC_build_fl(h, lw,
                        (ok -> ok_init || GC_debugging_started), 0);

              *result = op;
              GC_acquire_mark_lock();
              -- GC_fl_builder_count;
              if (GC_fl_builder_count == 0) GC_notify_all_builder();
              GC_release_mark_lock();
              (void) GC_clear_stack(0);
              return;
            }
#         endif
          op = GC_build_fl(h, lw, (ok -> ok_init || GC_debugging_started), 0);
          goto out;
        }
    }

     
     
      op = GC_generic_malloc_inner(lb, k);
      if (0 != op) obj_link(op) = 0;

  out:
    *result = op;
    UNLOCK();
    (void) GC_clear_stack(0);
}
