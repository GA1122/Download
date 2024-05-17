vrrp_sync_group_handler(vector_t *strvec)
{
	list l;
	element e;
	vrrp_sgroup_t *sg;
	char* gname;

	if (!strvec)
		return;

	if (vector_count(strvec) != 2) {
		report_config_error(CONFIG_GENERAL_ERROR, "vrrp_sync_group must have a name - skipping");
		skip_block(true);
		return;
	}

	gname = strvec_slot(strvec, 1);

	 
	if (!LIST_ISEMPTY(vrrp_data->vrrp_sync_group)) {
		l = vrrp_data->vrrp_sync_group;
		for (e = LIST_HEAD(l); e; ELEMENT_NEXT(e)) {
			sg = ELEMENT_DATA(e);
			if (!strcmp(gname,sg->gname)) {
				report_config_error(CONFIG_GENERAL_ERROR, "vrrp sync group %s already defined", gname);
				skip_block(true);
				return;
			}
		}
	}

	alloc_vrrp_sync_group(gname);
}