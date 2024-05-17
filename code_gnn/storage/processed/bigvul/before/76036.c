vrrp_strict_mode_handler(vector_t *strvec)
{
	int res;

	vrrp_t *vrrp = LIST_TAIL_DATA(vrrp_data->vrrp);
	if (vector_size(strvec) >= 2) {
		res = check_true_false(strvec_slot(strvec, 1));
		if (res >= 0)
			vrrp->strict_mode = (bool)res;
		else
			report_config_error(CONFIG_GENERAL_ERROR, "(%s) invalid strict_mode %s specified", vrrp->iname, FMT_STR_VSLOT(strvec, 1));
	} else {
		 
		vrrp->strict_mode = true;
	}
}