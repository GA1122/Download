 void perf_tp_event(u64 addr, u64 count, void *record, int entry_size,
 		   struct pt_regs *regs, struct hlist_head *head, int rctx)
 {
 	struct perf_sample_data data;
 	struct perf_event *event;
 	struct hlist_node *node;
 
 	struct perf_raw_record raw = {
 		.size = entry_size,
 		.data = record,
 	};
 
 	perf_sample_data_init(&data, addr);
 	data.raw = &raw;
  
  	hlist_for_each_entry_rcu(event, node, head, hlist_entry) {
  		if (perf_tp_event_match(event, &data, regs))
			perf_swevent_event(event, count, 1, &data, regs);
// 			perf_swevent_event(event, count, &data, regs);
  	}
  
  	perf_swevent_put_recursion_context(rctx);
 }