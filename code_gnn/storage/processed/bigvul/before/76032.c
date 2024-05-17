vrrp_skip_check_adv_addr_handler(vector_t *strvec)
{
	vrrp_t *vrrp = LIST_TAIL_DATA(vrrp_data->vrrp);
	int res;

	if (vector_size(strvec) >= 2) {
		res = check_true_false(strvec_slot(strvec, 1));
		if (res >= 0)
			vrrp->skip_check_adv_addr = (bool)res;
		else
			report_config_error(CONFIG_GENERAL_ERROR, "(%s) invalid skip_check_adv_addr %s specified", vrrp->iname, FMT_STR_VSLOT(strvec, 1));
	} else {
		 
		vrrp->skip_check_adv_addr = true;
	}
}