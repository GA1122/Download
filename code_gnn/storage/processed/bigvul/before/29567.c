static int ipc_memory_callback(struct notifier_block *self,
				unsigned long action, void *arg)
{
	static DECLARE_WORK(ipc_memory_wq, ipc_memory_notifier);

	switch (action) {
	case MEM_ONLINE:     
	case MEM_OFFLINE:    
		 
		if (!work_pending(&ipc_memory_wq))
			schedule_work(&ipc_memory_wq);
		break;
	case MEM_GOING_ONLINE:
	case MEM_GOING_OFFLINE:
	case MEM_CANCEL_ONLINE:
	case MEM_CANCEL_OFFLINE:
	default:
		break;
	}

	return NOTIFY_OK;
}
