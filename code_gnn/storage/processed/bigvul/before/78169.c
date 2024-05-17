static int asepcos_set_security_attributes(sc_card_t *card, sc_file_t *file)
{
	size_t i;
	const amode_entry_t *table;
	u8     buf[64], *p;
	int    r = SC_SUCCESS;

	 
	if (file->sec_attr != NULL && file->sec_attr_len != 0)
		return asepcos_set_sec_attributes(card, file->sec_attr,
				file->sec_attr_len, file->type == SC_FILE_TYPE_DF ? 0:1);
	 
	if (file->type == SC_FILE_TYPE_DF)
		table = df_amode_table;
	else if (file->type == SC_FILE_TYPE_WORKING_EF)
		table = wef_amode_table;
	else if (file->type == SC_FILE_TYPE_INTERNAL_EF)
		table = ief_amode_table;
	else
		return SC_ERROR_INVALID_ARGUMENTS;

	p = buf;
	for (i = 0; table[i].am != 0; i++) {
		const struct sc_acl_entry *ent = sc_file_get_acl_entry(file, table[i].sc);
		if (ent == NULL)
			continue;
		*p++ = 0x80;
		*p++ = 0x01;
		*p++ = table[i].am & 0xff;
		if (ent->method == SC_AC_NONE) {
			*p++ = 0x90;
			*p++ = 0x00;
		} else if (ent->method == SC_AC_NEVER) {
			*p++ = 0x97;
			*p++ = 0x00;
		} else if (ent->method == SC_AC_CHV) {
			sc_cardctl_asepcos_akn2fileid_t st;
			st.akn = ent->key_ref;
			r = asepcos_akn_to_fileid(card, &st);
			if (r != SC_SUCCESS)
				return r;
			*p++ = 0xa0;
			*p++ = 0x05;
			*p++ = 0x89;
			*p++ = 0x03;
			*p++ = (st.fileid >> 16) & 0xff;
			*p++ = (st.fileid >> 8 ) & 0xff;
			*p++ = st.fileid & 0xff;
		} else {
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "unknown auth method: '%d'", ent->method);
			return SC_ERROR_INTERNAL;
		} 
	}

	if (p != buf)
		r = asepcos_set_sec_attributes(card, buf, p-buf, file->type == SC_FILE_TYPE_DF ? 0:1);
	return r;
}