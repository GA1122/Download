 static void inotify_free_group_priv(struct fsnotify_group *group)
 {
 	 
  	idr_for_each(&group->inotify_data.idr, idr_callback, group);
  	idr_remove_all(&group->inotify_data.idr);
  	idr_destroy(&group->inotify_data.idr);
// 	atomic_dec(&group->inotify_data.user->inotify_devs);
  	free_uid(group->inotify_data.user);
  }