lockd_down(struct net *net)
{
	mutex_lock(&nlmsvc_mutex);
	lockd_down_net(nlmsvc_rqst->rq_server, net);
	if (nlmsvc_users) {
		if (--nlmsvc_users)
			goto out;
	} else {
		printk(KERN_ERR "lockd_down: no users! task=%p\n",
			nlmsvc_task);
		BUG();
	}

	if (!nlmsvc_task) {
		printk(KERN_ERR "lockd_down: no lockd running.\n");
		BUG();
	}
	kthread_stop(nlmsvc_task);
	dprintk("lockd_down: service stopped\n");
	lockd_svc_exit_thread();
	dprintk("lockd_down: service destroyed\n");
	nlmsvc_task = NULL;
	nlmsvc_rqst = NULL;
out:
	mutex_unlock(&nlmsvc_mutex);
}
