  static void prefetch_dec(void)
  {
//    
//   dec_tables.counter_head++;
//   dec_tables.counter_tail++;
// 
//    
    prefetch_table((const void *)&dec_tables, sizeof(dec_tables));
  }