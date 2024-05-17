clear_all_old(mrb_state *mrb, mrb_gc *gc)
{
  mrb_bool origin_mode = gc->generational;

  mrb_assert(is_generational(gc));
  if (is_major_gc(gc)) {
     
    incremental_gc_until(mrb, gc, MRB_GC_STATE_ROOT);
  }

   
  gc->generational = FALSE;
  prepare_incremental_sweep(mrb, gc);
  incremental_gc_until(mrb, gc, MRB_GC_STATE_ROOT);
  gc->generational = origin_mode;

   
  gc->atomic_gray_list = gc->gray_list = NULL;
}
