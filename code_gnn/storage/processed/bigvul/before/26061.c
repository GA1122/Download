void perf_event_comm(struct task_struct *task)
{
	struct perf_comm_event comm_event;
	struct perf_event_context *ctx;
	int ctxn;

	for_each_task_context_nr(ctxn) {
		ctx = task->perf_event_ctxp[ctxn];
		if (!ctx)
			continue;

		perf_event_enable_on_exec(ctx);
	}

	if (!atomic_read(&nr_comm_events))
		return;

	comm_event = (struct perf_comm_event){
		.task	= task,
		 
		 
		.event_id  = {
			.header = {
				.type = PERF_RECORD_COMM,
				.misc = 0,
				 
			},
			 
			 
		},
	};

	perf_event_comm_event(&comm_event);
}