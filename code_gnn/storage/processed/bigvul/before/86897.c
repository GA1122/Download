static TEE_Result tee_svc_obj_generate_key_ecc(
	struct tee_obj *o, const struct tee_cryp_obj_type_props *type_props,
	uint32_t key_size __unused,
	const TEE_Attribute *params, uint32_t param_count)
{
	TEE_Result res;
	struct ecc_keypair *tee_ecc_key;

	 
	res = tee_svc_cryp_obj_populate_type(o, type_props, params,
					     param_count);
	if (res != TEE_SUCCESS)
		return res;

	tee_ecc_key = (struct ecc_keypair *)o->attr;

	res = crypto_acipher_gen_ecc_key(tee_ecc_key);
	if (res != TEE_SUCCESS)
		return res;

	 
	set_attribute(o, type_props, TEE_ATTR_ECC_PRIVATE_VALUE);
	set_attribute(o, type_props, TEE_ATTR_ECC_PUBLIC_VALUE_X);
	set_attribute(o, type_props, TEE_ATTR_ECC_PUBLIC_VALUE_Y);
	set_attribute(o, type_props, TEE_ATTR_ECC_CURVE);
	return TEE_SUCCESS;
}