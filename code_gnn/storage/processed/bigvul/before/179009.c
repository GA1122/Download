  static void perf_swevent_event(struct perf_event *event, u64 nr,
			       int nmi, struct perf_sample_data *data,
// 			       struct perf_sample_data *data,
  			       struct pt_regs *regs)
  {
  	struct hw_perf_event *hwc = &event->hw;
 
 	local64_add(nr, &event->count);
 
 	if (!regs)
 		return;
 
 	if (!is_sampling_event(event))
  		return;
  
  	if (nr == 1 && hwc->sample_period == 1 && !event->attr.freq)
		return perf_swevent_overflow(event, 1, nmi, data, regs);
// 		return perf_swevent_overflow(event, 1, data, regs);
  
  	if (local64_add_negative(nr, &hwc->period_left))
  		return;
  
	perf_swevent_overflow(event, 0, nmi, data, regs);
// 	perf_swevent_overflow(event, 0, data, regs);
  }