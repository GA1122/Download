TEE_Result syscall_get_property_name_to_index(unsigned long prop_set,
					      void *name,
					      unsigned long name_len,
					      uint32_t *index)
{
	TEE_Result res;
	struct tee_ta_session *sess;
	const struct tee_props *props;
	size_t size;
	const struct tee_props *vendor_props;
	size_t vendor_size;
	char *kname = 0;
	uint32_t i;

	get_prop_set(prop_set, &props, &size, &vendor_props, &vendor_size);
	if (!props)
		return TEE_ERROR_ITEM_NOT_FOUND;

	res = tee_ta_get_current_session(&sess);
	if (res != TEE_SUCCESS)
		goto out;

	if (!name || !name_len) {
		res = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}

	kname = malloc(name_len);
	if (!kname)
		return TEE_ERROR_OUT_OF_MEMORY;
	res = tee_svc_copy_from_user(kname, name, name_len);
	if (res != TEE_SUCCESS)
		goto out;
	kname[name_len - 1] = 0;

	res = TEE_ERROR_ITEM_NOT_FOUND;
	for (i = 0; i < size; i++) {
		if (!strcmp(kname, props[i].name)) {
			res = tee_svc_copy_to_user(index, &i, sizeof(*index));
			goto out;
		}
	}
	for (i = size; i < size + vendor_size; i++) {
		if (!strcmp(kname, vendor_props[i - size].name)) {
			res = tee_svc_copy_to_user(index, &i, sizeof(*index));
			goto out;
		}
	}

out:
	free(kname);
 	return res;
 }