 static void perf_event_output(struct perf_event *event, int nmi,
// static void perf_event_output(struct perf_event *event,
  				struct perf_sample_data *data,
  				struct pt_regs *regs)
  {
 	struct perf_output_handle handle;
 	struct perf_event_header header;
 
 	 
 	rcu_read_lock();
  
  	perf_prepare_sample(&header, data, event, regs);
  
	if (perf_output_begin(&handle, event, header.size, nmi, 1))
// 	if (perf_output_begin(&handle, event, header.size, 1))
  		goto exit;
  
  	perf_output_sample(&handle, &header, data, event);
 
 	perf_output_end(&handle);
 
 exit:
 	rcu_read_unlock();
 }