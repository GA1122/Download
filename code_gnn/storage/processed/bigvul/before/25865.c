static int p4_validate_raw_event(struct perf_event *event)
{
	unsigned int v, emask;

	 
	v = p4_config_unpack_event(event->attr.config);
	if (v >= ARRAY_SIZE(p4_event_bind_map))
		return -EINVAL;

	 
	if (!p4_event_match_cpu_model(v))
		return -EINVAL;

	 

	 
	if (p4_ht_active() && p4_event_bind_map[v].shared) {
		if (perf_paranoid_cpu() && !capable(CAP_SYS_ADMIN))
			return -EACCES;
	}

	 
	emask = p4_config_unpack_escr(event->attr.config) & P4_ESCR_EVENTMASK_MASK;
	if (emask & ~p4_event_bind_map[v].escr_emask)
		return -EINVAL;

	 
	if (p4_config_pebs_has(event->attr.config, P4_PEBS_CONFIG_ENABLE))
		return -EINVAL;

	v = p4_config_unpack_metric(event->attr.config);
	if (v >= ARRAY_SIZE(p4_pebs_bind_map))
		return -EINVAL;

	return 0;
}
