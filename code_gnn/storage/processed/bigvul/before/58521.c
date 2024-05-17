void credssp_read_ts_credentials(rdpCredssp* credssp, PSecBuffer ts_credentials)
{
	wStream* s;
	int length;
	int ts_password_creds_length;

	s = Stream_New(ts_credentials->pvBuffer, ts_credentials->cbBuffer);

	 
	ber_read_sequence_tag(s, &length);

	 
	ber_read_contextual_tag(s, 0, &length, TRUE);
	ber_read_integer(s, NULL);

	 
	ber_read_contextual_tag(s, 1, &length, TRUE);
	ber_read_octet_string_tag(s, &ts_password_creds_length);

	credssp_read_ts_password_creds(credssp, s);

	Stream_Free(s, FALSE);
}