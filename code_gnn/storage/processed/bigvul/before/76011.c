vrrp_higher_prio_send_advert_handler(vector_t *strvec)
{
	int res;
	vrrp_t *vrrp = LIST_TAIL_DATA(vrrp_data->vrrp);

	if (vector_size(strvec) >= 2) {
		res = check_true_false(strvec_slot(strvec, 1));
		if (res >= 0)
			vrrp->higher_prio_send_advert = (unsigned)res;
		else
			report_config_error(CONFIG_GENERAL_ERROR, "(%s) invalid higher_prio_send_advert %s specified", vrrp->iname, FMT_STR_VSLOT(strvec, 1));
	} else {
		 
		vrrp->higher_prio_send_advert = true;
	}
}