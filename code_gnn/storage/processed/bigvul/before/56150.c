task_function_call(struct task_struct *p, remote_function_f func, void *info)
{
	struct remote_function_call data = {
		.p	= p,
		.func	= func,
		.info	= info,
		.ret	= -ESRCH,  
	};

	if (task_curr(p))
		smp_call_function_single(task_cpu(p), remote_function, &data, 1);

	return data.ret;
}