 void mm_release(struct task_struct *tsk, struct mm_struct *mm)
  {
  	struct completion *vfork_done = tsk->vfork_done;
  
// 	 
// #ifdef CONFIG_FUTEX
// 	if (unlikely(tsk->robust_list))
// 		exit_robust_list(tsk);
// #ifdef CONFIG_COMPAT
// 	if (unlikely(tsk->compat_robust_list))
// 		compat_exit_robust_list(tsk);
// #endif
// #endif
// 
  	 
  	deactivate_mm(tsk, mm);
  
 	 
 	if (vfork_done) {
 		tsk->vfork_done = NULL;
 		complete(vfork_done);
 	}
 
 	 
 	if (tsk->clear_child_tid
 	    && !(tsk->flags & PF_SIGNALED)
 	    && atomic_read(&mm->mm_users) > 1) {
 		u32 __user * tidptr = tsk->clear_child_tid;
 		tsk->clear_child_tid = NULL;
 
 		 
 		put_user(0, tidptr);
 		sys_futex(tidptr, FUTEX_WAKE, 1, NULL, NULL, 0);
 	}
 }