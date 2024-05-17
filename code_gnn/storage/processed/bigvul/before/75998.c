vrrp_garp_rep_handler(vector_t *strvec)
{
	vrrp_t *vrrp = LIST_TAIL_DATA(vrrp_data->vrrp);
	unsigned repeats;

	 
	if (!read_unsigned_strvec(strvec, 1, &repeats, 0, UINT_MAX, true)) {
		report_config_error(CONFIG_GENERAL_ERROR, "(%s): garp_master_repeat '%s' invalid - ignoring", vrrp->iname, FMT_STR_VSLOT(strvec, 1));
		return;
	}

	if (repeats == 0) {
		report_config_error(CONFIG_GENERAL_ERROR, "(%s): garp_master_repeat must be greater than 0, setting to 1", vrrp->iname);
		repeats = 1;
	}

	vrrp->garp_rep = repeats;
}