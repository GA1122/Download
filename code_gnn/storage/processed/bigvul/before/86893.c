static TEE_Result tee_svc_cryp_obj_populate_type(
		struct tee_obj *o,
		const struct tee_cryp_obj_type_props *type_props,
		const TEE_Attribute *attrs,
		uint32_t attr_count)
{
	TEE_Result res;
	uint32_t have_attrs = 0;
	size_t obj_size = 0;
	size_t n;
	int idx;
	const struct attr_ops *ops;
	void *attr;

	for (n = 0; n < attr_count; n++) {
		idx = tee_svc_cryp_obj_find_type_attr_idx(
							attrs[n].attributeID,
							type_props);
		 
		if (idx < 0)
			return TEE_ERROR_ITEM_NOT_FOUND;

		have_attrs |= BIT32(idx);
		ops = attr_ops + type_props->type_attrs[idx].ops_index;
		attr = (uint8_t *)o->attr +
		       type_props->type_attrs[idx].raw_offs;
		if (attrs[n].attributeID & TEE_ATTR_BIT_VALUE)
			res = ops->from_user(attr, &attrs[n].content.value,
					     sizeof(attrs[n].content.value));
		else
			res = ops->from_user(attr, attrs[n].content.ref.buffer,
					     attrs[n].content.ref.length);
		if (res != TEE_SUCCESS)
			return res;

		 
		if (type_props->type_attrs[idx].flags &
		    TEE_TYPE_ATTR_SIZE_INDICATOR) {
			 
			if (attrs[n].attributeID == TEE_ATTR_ECC_CURVE) {
				res = get_ec_key_size(attrs[n].content.value.a,
						      &obj_size);
				if (res != TEE_SUCCESS)
					return res;
			} else {
				obj_size += (attrs[n].content.ref.length * 8);
			}
		}
	}

	 
	if (o->info.objectType == TEE_TYPE_DES ||
	    o->info.objectType == TEE_TYPE_DES3)
		obj_size -= obj_size / 8;  

	o->have_attrs = have_attrs;
	o->info.keySize = obj_size;

	return TEE_SUCCESS;
}