 restore_page_device(i_ctx_t *i_ctx_p, const gs_gstate * pgs_old, const gs_gstate * pgs_new)
 {
     gx_device *dev_old = gs_currentdevice(pgs_old);
     gx_device *dev_new;
     gx_device *dev_t1;
     gx_device *dev_t2;
     bool samepagedevice = obj_eq(dev_old->memory, &gs_int_gstate(pgs_old)->pagedevice,
         &gs_int_gstate(pgs_new)->pagedevice);
     bool LockSafetyParams = dev_old->LockSafetyParams;
 
     if ((dev_t1 = (*dev_proc(dev_old, get_page_device)) (dev_old)) == 0)
         return 0;
      
      
      
     if (!samepagedevice)
         dev_old->LockSafetyParams = false;
     dev_new = gs_currentdevice(pgs_new);
     if (dev_old != dev_new) {
         if ((dev_t2 = (*dev_proc(dev_new, get_page_device)) (dev_new)) == 0)
             samepagedevice = true;
         else if (dev_t1 != dev_t2)
              samepagedevice = false;
      }
  
    if (LockSafetyParams && !samepagedevice) {
//     if (LockSafetyParams) {
          const int required_ops = 512;
          const int required_es = 32;
          
         if (required_ops + ref_stack_count(&o_stack) >= ref_stack_max_count(&o_stack)) {
            gs_currentdevice(pgs_old)->LockSafetyParams = LockSafetyParams;
            return_error(gs_error_stackoverflow);
         }
          
         if (required_es + ref_stack_count(&e_stack) >= ref_stack_max_count(&e_stack)) {
            gs_currentdevice(pgs_old)->LockSafetyParams = LockSafetyParams;
            return_error(gs_error_execstackoverflow);
         }
     }
      
     return samepagedevice ? 0 : 1;
 }