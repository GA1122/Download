static long perf_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct perf_event *event = file->private_data;
	void (*func)(struct perf_event *);
	u32 flags = arg;

	switch (cmd) {
	case PERF_EVENT_IOC_ENABLE:
		func = perf_event_enable;
		break;
	case PERF_EVENT_IOC_DISABLE:
		func = perf_event_disable;
		break;
	case PERF_EVENT_IOC_RESET:
		func = perf_event_reset;
		break;

	case PERF_EVENT_IOC_REFRESH:
		return perf_event_refresh(event, arg);

	case PERF_EVENT_IOC_PERIOD:
		return perf_event_period(event, (u64 __user *)arg);

	case PERF_EVENT_IOC_SET_OUTPUT:
	{
		struct perf_event *output_event = NULL;
		int fput_needed = 0;
		int ret;

		if (arg != -1) {
			output_event = perf_fget_light(arg, &fput_needed);
			if (IS_ERR(output_event))
				return PTR_ERR(output_event);
		}

		ret = perf_event_set_output(event, output_event);
		if (output_event)
			fput_light(output_event->filp, fput_needed);

		return ret;
	}

	case PERF_EVENT_IOC_SET_FILTER:
		return perf_event_set_filter(event, (void __user *)arg);

	default:
		return -ENOTTY;
	}

	if (flags & PERF_IOC_FLAG_GROUP)
		perf_event_for_each(event, func);
	else
		perf_event_for_each_child(event, func);

	return 0;
}
