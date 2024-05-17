TEE_Result tee_obj_attr_to_binary(struct tee_obj *o, void *data,
				  size_t *data_len)
{
	const struct tee_cryp_obj_type_props *tp;
	size_t n;
	size_t offs = 0;
	size_t len = data ? *data_len : 0;
	TEE_Result res;

	if (o->info.objectType == TEE_TYPE_DATA) {
		*data_len = 0;
		return TEE_SUCCESS;  
	}
	if (!o->attr)
		return TEE_ERROR_BAD_STATE;
	tp = tee_svc_find_type_props(o->info.objectType);
	if (!tp)
		return TEE_ERROR_BAD_STATE;

	for (n = 0; n < tp->num_type_attrs; n++) {
		const struct tee_cryp_obj_type_attrs *ta = tp->type_attrs + n;
		void *attr = (uint8_t *)o->attr + ta->raw_offs;

		res = attr_ops[ta->ops_index].to_binary(attr, data, len, &offs);
		if (res != TEE_SUCCESS)
			return res;
	}

	*data_len = offs;
	if (data && offs > len)
		return TEE_ERROR_SHORT_BUFFER;
	return TEE_SUCCESS;
}