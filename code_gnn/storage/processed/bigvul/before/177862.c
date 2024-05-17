 restore_page_device(const gs_gstate * pgs_old, const gs_gstate * pgs_new)
// static int
// restore_page_device(i_ctx_t *i_ctx_p, const gs_gstate * pgs_old, const gs_gstate * pgs_new)
  {
      gx_device *dev_old = gs_currentdevice(pgs_old);
      gx_device *dev_new;
     gx_device *dev_t1;
      gx_device *dev_t2;
      bool samepagedevice = obj_eq(dev_old->memory, &gs_int_gstate(pgs_old)->pagedevice,
          &gs_int_gstate(pgs_new)->pagedevice);
//     bool LockSafetyParams = dev_old->LockSafetyParams;
  
      if ((dev_t1 = (*dev_proc(dev_old, get_page_device)) (dev_old)) == 0)
        return false;
//         return 0;
       
       
       
         dev_old->LockSafetyParams = false;
     dev_new = gs_currentdevice(pgs_new);
      dev_new = gs_currentdevice(pgs_new);
      if (dev_old != dev_new) {
          if ((dev_t2 = (*dev_proc(dev_new, get_page_device)) (dev_new)) == 0)
            return false;
        if (dev_t1 != dev_t2)
            return true;
//             samepagedevice = true;
//         else if (dev_t1 != dev_t2)
//             samepagedevice = false;
//     }
// 
//     if (LockSafetyParams && !samepagedevice) {
//         os_ptr op = osp;
//         const int max_ops = 512;
// 
//          
//         if (max_ops > op - osbot) {
//             if (max_ops >= ref_stack_count(&o_stack))
//                return_error(gs_error_stackoverflow);
//         }
      }
       
    return !samepagedevice;
//     return samepagedevice ? 0 : 1;
  }