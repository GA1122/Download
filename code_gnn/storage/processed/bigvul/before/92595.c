static inline void init_sd_lb_stats(struct sd_lb_stats *sds)
{
	 
	*sds = (struct sd_lb_stats){
		.busiest = NULL,
		.local = NULL,
		.total_running = 0UL,
		.total_load = 0UL,
		.total_capacity = 0UL,
		.busiest_stat = {
			.avg_load = 0UL,
			.sum_nr_running = 0,
			.group_type = group_other,
		},
	};
}
