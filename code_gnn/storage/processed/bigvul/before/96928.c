static int tracing_wait_pipe(struct file *filp)
{
	struct trace_iterator *iter = filp->private_data;
	int ret;

	while (trace_empty(iter)) {

		if ((filp->f_flags & O_NONBLOCK)) {
			return -EAGAIN;
		}

		 
		if (!tracer_tracing_is_on(iter->tr) && iter->pos)
			break;

		mutex_unlock(&iter->mutex);

		ret = wait_on_pipe(iter, 0);

		mutex_lock(&iter->mutex);

		if (ret)
			return ret;
	}

	return 1;
}
