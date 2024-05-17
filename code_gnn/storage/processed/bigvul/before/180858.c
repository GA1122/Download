  void uwbd_stop(struct uwb_rc *rc)
  {
	kthread_stop(rc->uwbd.task);
// 	if (rc->uwbd.task)
// 		kthread_stop(rc->uwbd.task);
  	uwbd_flush(rc);
  }