  void uwbd_start(struct uwb_rc *rc)
  {
	rc->uwbd.task = kthread_run(uwbd, rc, "uwbd");
	if (rc->uwbd.task == NULL)
// 	struct task_struct *task = kthread_run(uwbd, rc, "uwbd");
// 	if (IS_ERR(task)) {
// 		rc->uwbd.task = NULL;
  		printk(KERN_ERR "UWB: Cannot start management daemon; "
  		       "UWB won't work\n");
	else
// 	} else {
// 		rc->uwbd.task = task;
  		rc->uwbd.pid = rc->uwbd.task->pid;
// 	}
  }