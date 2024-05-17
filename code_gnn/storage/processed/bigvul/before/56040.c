static int cpu_function_call(int cpu, remote_function_f func, void *info)
{
	struct remote_function_call data = {
		.p	= NULL,
		.func	= func,
		.info	= info,
		.ret	= -ENXIO,  
	};

	smp_call_function_single(cpu, remote_function, &data, 1);

	return data.ret;
}