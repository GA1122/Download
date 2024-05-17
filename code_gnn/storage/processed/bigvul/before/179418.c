 int do_set_thread_area(struct task_struct *p, int idx,
 		       struct user_desc __user *u_info,
 		       int can_allocate)
 {
 	struct user_desc info;
 
  	if (copy_from_user(&info, u_info, sizeof(info)))
  		return -EFAULT;
  
// 	if (!tls_desc_okay(&info))
// 		return -EINVAL;
// 
  	if (idx == -1)
  		idx = info.entry_number;
  
 	 
 	if (idx == -1 && can_allocate) {
 		idx = get_free_idx();
 		if (idx < 0)
 			return idx;
 		if (put_user(idx, &u_info->entry_number))
 			return -EFAULT;
 	}
 
 	if (idx < GDT_ENTRY_TLS_MIN || idx > GDT_ENTRY_TLS_MAX)
 		return -EINVAL;
 
 	set_tls_desc(p, idx, &info, 1);
 
 	return 0;
 }