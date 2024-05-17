static TEE_Result tee_svc_cryp_check_attr(enum attr_usage usage,
					  const struct tee_cryp_obj_type_props
						*type_props,
					  const TEE_Attribute *attrs,
					  uint32_t attr_count)
{
	uint32_t required_flag;
	uint32_t opt_flag;
	bool all_opt_needed;
	uint32_t req_attrs = 0;
	uint32_t opt_grp_attrs = 0;
	uint32_t attrs_found = 0;
	size_t n;
	uint32_t bit;
	uint32_t flags;
	int idx;

	if (usage == ATTR_USAGE_POPULATE) {
		required_flag = TEE_TYPE_ATTR_REQUIRED;
		opt_flag = TEE_TYPE_ATTR_OPTIONAL_GROUP;
		all_opt_needed = true;
	} else {
		required_flag = TEE_TYPE_ATTR_GEN_KEY_REQ;
		opt_flag = TEE_TYPE_ATTR_GEN_KEY_OPT;
		all_opt_needed = false;
	}

	 
	for (n = 0; n < type_props->num_type_attrs; n++) {
		bit = 1 << n;
		flags = type_props->type_attrs[n].flags;

		if (flags & required_flag)
			req_attrs |= bit;
		else if (flags & opt_flag)
			opt_grp_attrs |= bit;
	}

	 
	for (n = 0; n < attr_count; n++) {
		idx = tee_svc_cryp_obj_find_type_attr_idx(
							attrs[n].attributeID,
							type_props);

		 
		if (idx < 0)
			return TEE_ERROR_ITEM_NOT_FOUND;

		bit = 1 << idx;

		 
		if ((attrs_found & bit) != 0)
			return TEE_ERROR_ITEM_NOT_FOUND;

		attrs_found |= bit;
	}
	 
	if ((attrs_found & req_attrs) != req_attrs)
		return TEE_ERROR_ITEM_NOT_FOUND;

	 
	if (all_opt_needed && (attrs_found & opt_grp_attrs) != 0 &&
	    (attrs_found & opt_grp_attrs) != opt_grp_attrs)
		return TEE_ERROR_ITEM_NOT_FOUND;

	return TEE_SUCCESS;
}