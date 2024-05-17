trace_event_buffer_lock_reserve(struct ring_buffer **current_rb,
			  struct trace_event_file *trace_file,
			  int type, unsigned long len,
			  unsigned long flags, int pc)
{
	struct ring_buffer_event *entry;
	int val;

	*current_rb = trace_file->tr->trace_buffer.buffer;

	if (!ring_buffer_time_stamp_abs(*current_rb) && (trace_file->flags &
	     (EVENT_FILE_FL_SOFT_DISABLED | EVENT_FILE_FL_FILTERED)) &&
	    (entry = this_cpu_read(trace_buffered_event))) {
		 
		val = this_cpu_inc_return(trace_buffered_event_cnt);
		if (val == 1) {
			trace_event_setup(entry, type, flags, pc);
			entry->array[0] = len;
			return entry;
		}
		this_cpu_dec(trace_buffered_event_cnt);
	}

	entry = __trace_buffer_lock_reserve(*current_rb,
					    type, len, flags, pc);
	 
	if (!entry && trace_file->flags & EVENT_FILE_FL_TRIGGER_COND) {
		*current_rb = temp_buffer;
		entry = __trace_buffer_lock_reserve(*current_rb,
						    type, len, flags, pc);
	}
	return entry;
}
