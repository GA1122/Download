TEE_Result syscall_get_property(unsigned long prop_set,
				unsigned long index,
				void *name, uint32_t *name_len,
				void *buf, uint32_t *blen,
				uint32_t *prop_type)
{
	struct tee_ta_session *sess;
	TEE_Result res;
	TEE_Result res2;
	const struct tee_props *prop;
	uint32_t klen;
	size_t klen_size;
	uint32_t elen;

	prop = get_prop_struct(prop_set, index);
	if (!prop)
		return TEE_ERROR_ITEM_NOT_FOUND;

	res = tee_ta_get_current_session(&sess);
	if (res != TEE_SUCCESS)
		return res;

	 
	if (prop_type) {
		res = tee_svc_copy_to_user(prop_type, &prop->prop_type,
					   sizeof(*prop_type));
		if (res != TEE_SUCCESS)
			return res;
	}

	 
	if (buf && blen) {
		res = tee_svc_copy_from_user(&klen, blen, sizeof(klen));
		if (res != TEE_SUCCESS)
			return res;

		if (prop->get_prop_func) {
			klen_size = klen;
			res = prop->get_prop_func(sess, buf, &klen_size);
			klen = klen_size;
			res2 = tee_svc_copy_to_user(blen, &klen, sizeof(*blen));
		} else {
			if (klen < prop->len)
				res = TEE_ERROR_SHORT_BUFFER;
			else
				res = tee_svc_copy_to_user(buf, prop->data,
							   prop->len);
			res2 = tee_svc_copy_to_user(blen, &prop->len,
						    sizeof(*blen));
		}
		if (res2 != TEE_SUCCESS)
			return res2;
		if (res != TEE_SUCCESS)
			return res;
	}

	 
	if (name && name_len) {
		res = tee_svc_copy_from_user(&klen, name_len, sizeof(klen));
		if (res != TEE_SUCCESS)
			return res;

		elen = strlen(prop->name) + 1;

		if (klen < elen)
			res = TEE_ERROR_SHORT_BUFFER;
		else
			res = tee_svc_copy_to_user(name, prop->name, elen);
		res2 = tee_svc_copy_to_user(name_len, &elen, sizeof(*name_len));
		if (res2 != TEE_SUCCESS)
			return res2;
		if (res != TEE_SUCCESS)
			return res;
	}

	return res;
}