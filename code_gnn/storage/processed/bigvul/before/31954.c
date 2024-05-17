static void perf_event_task_output(struct perf_event *event,
				     struct perf_task_event *task_event)
{
	struct perf_output_handle handle;
	struct perf_sample_data	sample;
	struct task_struct *task = task_event->task;
	int ret, size = task_event->event_id.header.size;

	perf_event_header__init_id(&task_event->event_id.header, &sample, event);

	ret = perf_output_begin(&handle, event,
				task_event->event_id.header.size);
	if (ret)
		goto out;

	task_event->event_id.pid = perf_event_pid(event, task);
	task_event->event_id.ppid = perf_event_pid(event, current);

	task_event->event_id.tid = perf_event_tid(event, task);
	task_event->event_id.ptid = perf_event_tid(event, current);

	perf_output_put(&handle, task_event->event_id);

	perf_event__output_id_sample(event, &handle, &sample);

	perf_output_end(&handle);
out:
	task_event->event_id.header.size = size;
}