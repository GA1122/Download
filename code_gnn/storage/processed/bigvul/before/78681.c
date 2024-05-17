static int set_sec_attr_from_acl(sc_card_t *card, sc_file_t *file)
{
	const sc_acl_entry_t *entry;
	u8 sec_attr[SC_RTECP_SEC_ATTR_SIZE] = { 0 };
	int r;

	assert(card && card->ctx && file);
	assert(!file->sec_attr  &&  file->sec_attr_len == 0);
	assert(1 + 6 < sizeof(sec_attr));

	entry = sc_file_get_acl_entry(file, SC_AC_OP_DELETE);
	if (entry)
	{
		sec_attr[0] |= 0x40;
		sec_attr[1 + 6] = to_sec_attr(entry->method, entry->key_ref);
	}
	if (file->type == SC_FILE_TYPE_DF)
	{
		entry = sc_file_get_acl_entry(file, SC_AC_OP_CREATE);
		if (entry)
		{
			 
			sec_attr[0] |= 0x01;
			sec_attr[1 + 0] = to_sec_attr(entry->method, entry->key_ref);
			 
			sec_attr[0] |= 0x02;
			sec_attr[1 + 1] = to_sec_attr(entry->method, entry->key_ref);
		}
	}
	else
	{
		entry = sc_file_get_acl_entry(file, SC_AC_OP_READ);
		if (entry)
		{
			sec_attr[0] |= 0x01;
			sec_attr[1 + 0] = to_sec_attr(entry->method, entry->key_ref);
		}
		entry = sc_file_get_acl_entry(file, SC_AC_OP_WRITE);
		if (entry)
		{
			sec_attr[0] |= 0x02;
			sec_attr[1 + 1] = to_sec_attr(entry->method, entry->key_ref);
		}
		entry = sc_file_get_acl_entry(file, SC_AC_OP_UPDATE);
		if (entry)
		{
			 
			sec_attr[0] |= 0x02;
			sec_attr[1 + 1] = to_sec_attr(entry->method, entry->key_ref);
		}
	}
	 
	if (file->path.len == 2 && !memcmp(file->path.value, "\x3F\x00", 2))
	{
		 
		sec_attr[0] |= 0x04;
		sec_attr[1 + 2] = 1;  
	}
	r = sc_file_set_sec_attr(file, sec_attr, sizeof(sec_attr));
	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, r);
}