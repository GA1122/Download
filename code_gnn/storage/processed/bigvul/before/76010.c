vrrp_handler(vector_t *strvec)
{
	list l;
	element e;
	vrrp_t *vrrp;
	char *iname;

	global_data->have_vrrp_config = true;

	if (!strvec)
		return;

	if (vector_count(strvec) != 2) {
		report_config_error(CONFIG_GENERAL_ERROR, "vrrp_instance must have a name");
		skip_block(true);
		return;
	}

	iname = strvec_slot(strvec,1);

	 
	if (!LIST_ISEMPTY(vrrp_data->vrrp)) {
		l = vrrp_data->vrrp;
		for (e = LIST_HEAD(l); e; ELEMENT_NEXT(e)) {
			vrrp = ELEMENT_DATA(e);
			if (!strcmp(iname,vrrp->iname)) {
				report_config_error(CONFIG_GENERAL_ERROR, "vrrp instance %s already defined", iname );
				skip_block(true);
				return;
			}
		}
	}

	alloc_vrrp(iname);
}