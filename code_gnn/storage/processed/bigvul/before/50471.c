void perf_event_comm(struct task_struct *task, bool exec)
{
	struct perf_comm_event comm_event;

	if (!atomic_read(&nr_comm_events))
		return;

	comm_event = (struct perf_comm_event){
		.task	= task,
		 
		 
		.event_id  = {
			.header = {
				.type = PERF_RECORD_COMM,
				.misc = exec ? PERF_RECORD_MISC_COMM_EXEC : 0,
				 
			},
			 
			 
		},
	};

	perf_event_comm_event(&comm_event);
}
