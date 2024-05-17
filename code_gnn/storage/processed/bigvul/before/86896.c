static TEE_Result tee_svc_obj_generate_key_dsa(
	struct tee_obj *o, const struct tee_cryp_obj_type_props *type_props,
	uint32_t key_size)
{
	TEE_Result res;

	res = crypto_acipher_gen_dsa_key(o->attr, key_size);
	if (res != TEE_SUCCESS)
		return res;

	 
	o->have_attrs = (1 << type_props->num_type_attrs) - 1;

	return TEE_SUCCESS;
}