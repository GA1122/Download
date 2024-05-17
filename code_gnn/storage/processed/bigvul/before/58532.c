int credssp_write_ts_credentials(rdpCredssp* credssp, wStream* s)
{
	int size = 0;
	int innerSize = credssp_sizeof_ts_credentials(credssp);
	int passwordSize;

	 
	size += ber_write_sequence_tag(s, innerSize);

	 
	size += ber_write_contextual_tag(s, 0, ber_sizeof_integer(1), TRUE);
	size += ber_write_integer(s, 1);

	 

	passwordSize = ber_sizeof_sequence(credssp_sizeof_ts_password_creds(credssp));

	size += ber_write_contextual_tag(s, 1, ber_sizeof_octet_string(passwordSize), TRUE);
	size += ber_write_octet_string_tag(s, passwordSize);
	size += credssp_write_ts_password_creds(credssp, s);

	return size;
}