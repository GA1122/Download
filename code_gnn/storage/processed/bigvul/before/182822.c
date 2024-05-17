  do_prefetch_tables (const void *gcmM, size_t gcmM_size)
  {
//    
//   gcm_table.counter_head++;
//   gcm_table.counter_tail++;
// 
//    
    prefetch_table(gcmM, gcmM_size);
  prefetch_table(gcmR, sizeof(gcmR));
//   prefetch_table(&gcm_table, sizeof(gcm_table));
  }