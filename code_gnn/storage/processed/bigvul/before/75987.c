vrrp_adv_handler(vector_t *strvec)
{
	vrrp_t *vrrp = LIST_TAIL_DATA(vrrp_data->vrrp);
	double adver_int;
	bool res;

	res = read_double_strvec(strvec, 1, &adver_int, 0.01, 255.0, true);

	 
	if (!res || adver_int <= 0)
		report_config_error(CONFIG_GENERAL_ERROR, "(%s) Advert interval (%s) not valid! Must be > 0 - ignoring", vrrp->iname, FMT_STR_VSLOT(strvec, 1));
	else
		vrrp->adver_int = (unsigned)(adver_int * TIMER_HZ);
}