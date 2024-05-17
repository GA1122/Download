static TEE_Result tee_svc_obj_generate_key_dh(
	struct tee_obj *o, const struct tee_cryp_obj_type_props *type_props,
	uint32_t key_size __unused,
	const TEE_Attribute *params, uint32_t param_count)
{
	TEE_Result res;
	struct dh_keypair *tee_dh_key;
	struct bignum *dh_q = NULL;
	uint32_t dh_xbits = 0;

	 
	res = tee_svc_cryp_obj_populate_type(o, type_props, params,
					     param_count);
	if (res != TEE_SUCCESS)
		return res;

	tee_dh_key = (struct dh_keypair *)o->attr;

	if (get_attribute(o, type_props, TEE_ATTR_DH_SUBPRIME))
		dh_q = tee_dh_key->q;
	if (get_attribute(o, type_props, TEE_ATTR_DH_X_BITS))
		dh_xbits = tee_dh_key->xbits;
	res = crypto_acipher_gen_dh_key(tee_dh_key, dh_q, dh_xbits);
	if (res != TEE_SUCCESS)
		return res;

	 
	set_attribute(o, type_props, TEE_ATTR_DH_PUBLIC_VALUE);
	set_attribute(o, type_props, TEE_ATTR_DH_PRIVATE_VALUE);
	set_attribute(o, type_props, TEE_ATTR_DH_X_BITS);
	return TEE_SUCCESS;
}