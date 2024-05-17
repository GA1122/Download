 void exit_io_context(void)
// void exit_io_context(struct task_struct *task)
  {
  	struct io_context *ioc;
  
	task_lock(current);
	ioc = current->io_context;
	current->io_context = NULL;
	task_unlock(current);
// 	task_lock(task);
// 	ioc = task->io_context;
// 	task->io_context = NULL;
// 	task_unlock(task);
  
  	if (atomic_dec_and_test(&ioc->nr_tasks)) {
  		if (ioc->aic && ioc->aic->exit)
 			ioc->aic->exit(ioc->aic);
 		cfq_exit(ioc);
 
 	}
 	put_io_context(ioc);
 }