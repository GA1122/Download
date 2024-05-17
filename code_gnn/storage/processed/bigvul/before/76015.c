vrrp_lower_prio_no_advert_handler(vector_t *strvec)
{
	vrrp_t *vrrp = LIST_TAIL_DATA(vrrp_data->vrrp);
	int res;

	if (vector_size(strvec) >= 2) {
		res = check_true_false(strvec_slot(strvec, 1));
		if (res >= 0)
			vrrp->lower_prio_no_advert = (unsigned)res;
		else
			report_config_error(CONFIG_GENERAL_ERROR, "(%s) invalid lower_prio_no_advert %s specified", vrrp->iname, FMT_STR_VSLOT(strvec, 1));
	} else {
		 
		vrrp->lower_prio_no_advert = true;
	}
}