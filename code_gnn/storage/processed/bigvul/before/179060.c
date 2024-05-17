 static struct fsnotify_group *inotify_new_group(struct user_struct *user, unsigned int max_events)
// static struct fsnotify_group *inotify_new_group(unsigned int max_events)
  {
  	struct fsnotify_group *group;
  
 	group = fsnotify_alloc_group(&inotify_fsnotify_ops);
 	if (IS_ERR(group))
 		return group;
 
 	group->max_events = max_events;
 
  	spin_lock_init(&group->inotify_data.idr_lock);
  	idr_init(&group->inotify_data.idr);
  	group->inotify_data.last_wd = 0;
	group->inotify_data.user = user;
  	group->inotify_data.fa = NULL;
// 	group->inotify_data.user = get_current_user();
// 
// 	if (atomic_inc_return(&group->inotify_data.user->inotify_devs) >
// 	    inotify_max_user_instances) {
// 		fsnotify_put_group(group);
// 		return ERR_PTR(-EMFILE);
// 	}
  
  	return group;
  }