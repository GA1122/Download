garp_group_interfaces_handler(vector_t *strvec)
{
	garp_delay_t *delay = LIST_TAIL_DATA(garp_delay);
	interface_t *ifp;
	vector_t *interface_vec = read_value_block(strvec);
	size_t i;
	garp_delay_t *gd;
	element e;

	 
	if (!interface_vec) {
		report_config_error(CONFIG_GENERAL_ERROR, "Warning - empty garp_group interfaces block");
		return;
	}

	 
	delay->aggregation_group = 1;
	for (e = LIST_HEAD(garp_delay); e; ELEMENT_NEXT(e)) {
		gd = ELEMENT_DATA(e);
		if (gd->aggregation_group && gd != delay)
			delay->aggregation_group++;
	}

	for (i = 0; i < vector_size(interface_vec); i++) {
		ifp = if_get_by_ifname(vector_slot(interface_vec, i), IF_CREATE_IF_DYNAMIC);
		if (!ifp) {
			if (global_data->dynamic_interfaces)
				log_message(LOG_INFO, "WARNING - interface %s specified for garp_group doesn't exist", FMT_STR_VSLOT(strvec, i));
			else
				report_config_error(CONFIG_GENERAL_ERROR, "WARNING - interface %s specified for garp_group doesn't exist", FMT_STR_VSLOT(strvec, i));
			continue;
		}

		if (ifp->garp_delay) {
			report_config_error(CONFIG_GENERAL_ERROR, "garp_group already specified for %s - ignoring", FMT_STR_VSLOT(strvec, 1));
			continue;
		}

#ifdef _HAVE_VRRP_VMAC_
		if (ifp->vmac_type) {
			report_config_error(CONFIG_GENERAL_ERROR, "Cannot specify garp_delay on a vmac (%s) - ignoring", ifp->ifname);
			continue;
		}
#endif
		ifp->garp_delay = delay;
	}

	free_strvec(interface_vec);
}