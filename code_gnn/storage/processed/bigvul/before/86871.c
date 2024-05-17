TEE_Result syscall_cryp_obj_get_attr(unsigned long obj, unsigned long attr_id,
			void *buffer, uint64_t *size)
{
	TEE_Result res;
	struct tee_ta_session *sess;
	struct tee_obj *o;
	const struct tee_cryp_obj_type_props *type_props;
	int idx;
	const struct attr_ops *ops;
	void *attr;

	res = tee_ta_get_current_session(&sess);
	if (res != TEE_SUCCESS)
		return res;

	res = tee_obj_get(to_user_ta_ctx(sess->ctx),
			  tee_svc_uref_to_vaddr(obj), &o);
	if (res != TEE_SUCCESS)
		return TEE_ERROR_ITEM_NOT_FOUND;

	 
	if (!(o->info.handleFlags & TEE_HANDLE_FLAG_INITIALIZED))
		return TEE_ERROR_BAD_PARAMETERS;

	 
	if (!(attr_id & TEE_ATTR_BIT_PROTECTED) &&
	    !(o->info.objectUsage & TEE_USAGE_EXTRACTABLE))
		return TEE_ERROR_BAD_PARAMETERS;

	type_props = tee_svc_find_type_props(o->info.objectType);
	if (!type_props) {
		 
		return TEE_ERROR_BAD_STATE;
	}

	idx = tee_svc_cryp_obj_find_type_attr_idx(attr_id, type_props);
	if ((idx < 0) || ((o->have_attrs & (1 << idx)) == 0))
		return TEE_ERROR_ITEM_NOT_FOUND;

	ops = attr_ops + type_props->type_attrs[idx].ops_index;
	attr = (uint8_t *)o->attr + type_props->type_attrs[idx].raw_offs;
	return ops->to_user(attr, sess, buffer, size);
}