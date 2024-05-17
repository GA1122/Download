static void remote_function(void *data)
{
	struct remote_function_call *tfc = data;
	struct task_struct *p = tfc->p;

	if (p) {
		 
		if (task_cpu(p) != smp_processor_id())
			return;

		 

		tfc->ret = -ESRCH;  
		if (p != current)
			return;
	}

	tfc->ret = tfc->func(tfc->info);
}
