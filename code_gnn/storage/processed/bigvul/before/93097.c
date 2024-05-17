sec_connect(char *server, char *username, char *domain, char *password, RD_BOOL reconnect)
{
	uint32 selected_proto;
	struct stream mcs_data;

	 
	if (!mcs_connect_start(server, username, domain, password, reconnect, &selected_proto))
		return False;

	 
	mcs_data.size = 512;
	mcs_data.p = mcs_data.data = (uint8 *) xmalloc(mcs_data.size);
	sec_out_mcs_connect_initial_pdu(&mcs_data, selected_proto);

	 
	if (!mcs_connect_finalize(&mcs_data))
		return False;

	 
	if (g_encryption)
		sec_establish_key();
	xfree(mcs_data.data);
	return True;
}
