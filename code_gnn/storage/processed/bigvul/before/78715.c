static int itacns_add_pubkey(sc_pkcs15_card_t *p15card,
	 const sc_path_t *path, const sc_pkcs15_id_t *id, const char *label,
	int usage, int ref, int obj_flags, int *modulus_len_out)
{
	int r;
	sc_pkcs15_pubkey_info_t info;
	sc_pkcs15_object_t obj;

	SC_FUNC_CALLED(p15card->card->ctx, 1);

	memset(&info, 0, sizeof(info));
	memset(&obj,  0, sizeof(obj));

	info.id  		= *id;
	if (path)
		info.path	= *path;
	info.usage		= usage;
	info.key_reference	= ref;
	strlcpy(obj.label, label, sizeof(obj.label));
	obj.flags		= obj_flags;

	 
	info.modulus_length = 1024;

	*modulus_len_out = info.modulus_length;
	r = sc_pkcs15emu_add_rsa_pubkey(p15card, &obj, &info);
	SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
		"Could not add pub key");
	return r;
}