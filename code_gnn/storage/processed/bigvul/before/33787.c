static long vhost_dev_set_owner(struct vhost_dev *dev)
{
	struct task_struct *worker;
	int err;

	 
	if (dev->mm) {
		err = -EBUSY;
		goto err_mm;
	}

	 
	dev->mm = get_task_mm(current);
	worker = kthread_create(vhost_worker, dev, "vhost-%d", current->pid);
	if (IS_ERR(worker)) {
		err = PTR_ERR(worker);
		goto err_worker;
	}

	dev->worker = worker;
	wake_up_process(worker);	 

	err = vhost_attach_cgroups(dev);
	if (err)
		goto err_cgroup;

	err = vhost_dev_alloc_iovecs(dev);
	if (err)
		goto err_cgroup;

	return 0;
err_cgroup:
	kthread_stop(worker);
	dev->worker = NULL;
err_worker:
	if (dev->mm)
		mmput(dev->mm);
	dev->mm = NULL;
err_mm:
	return err;
}