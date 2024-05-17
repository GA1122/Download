 int arch_dup_task_struct(struct task_struct *dst, struct task_struct *src)
 {
 	flush_fp_to_thread(src);
  	flush_altivec_to_thread(src);
  	flush_vsx_to_thread(src);
  	flush_spe_to_thread(src);
// 	 
// 	__switch_to_tm(src);
// 	tm_recheckpoint_new_task(src);
  
  	*dst = *src;
  
 	clear_task_ebb(dst);
 
 	return 0;
 }