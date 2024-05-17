garp_group_end_handler(void)
{
	garp_delay_t *delay = LIST_TAIL_DATA(garp_delay);
	element e, next;
	interface_t *ifp;

	if (!delay->have_garp_interval && !delay->have_gna_interval) {
		report_config_error(CONFIG_GENERAL_ERROR, "garp group %d does not have any delay set - removing", delay->aggregation_group);

		 
		LIST_FOREACH_NEXT(get_if_list(), ifp, e, next) {
			if (ifp->garp_delay == delay)
				ifp->garp_delay = NULL;
		}

		free_list_element(garp_delay, garp_delay->tail);
	}
}