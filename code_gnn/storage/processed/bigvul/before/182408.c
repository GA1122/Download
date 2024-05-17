 static void *etm_setup_aux(int event_cpu, void **pages,
 			   int nr_pages, bool overwrite)
 {
 	int cpu;
 	cpumask_t *mask;
 	struct coresight_device *sink;
 	struct etm_event_data *event_data = NULL;
 
 	event_data = alloc_event_data(event_cpu);
 	if (!event_data)
 		return NULL;
 
 	 
 	sink = coresight_get_enabled_sink(true);
 	if (!sink)
 		goto err;
 
 	INIT_WORK(&event_data->work, free_event_data);
 
 	mask = &event_data->mask;
 
 	 
 	for_each_cpu(cpu, mask) {
 		struct coresight_device *csdev;
 
 		csdev = per_cpu(csdev_src, cpu);
 		if (!csdev)
 			goto err;
 
 		 
 		event_data->path[cpu] = coresight_build_path(csdev, sink);
 		if (IS_ERR(event_data->path[cpu]))
 			goto err;
 	}
 
  	if (!sink_ops(sink)->alloc_buffer)
  		goto err;
  
// 	cpu = cpumask_first(mask);
  	 
  	event_data->snk_config =
  			sink_ops(sink)->alloc_buffer(sink, cpu, pages,
 						     nr_pages, overwrite);
 	if (!event_data->snk_config)
 		goto err;
 
 out:
 	return event_data;
 
 err:
 	etm_free_aux(event_data);
 	event_data = NULL;
 	goto out;
 }