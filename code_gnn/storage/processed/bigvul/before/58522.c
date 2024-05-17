void credssp_read_ts_password_creds(rdpCredssp* credssp, wStream* s)
{
	int length;

	 
	ber_read_sequence_tag(s, &length);

	 
	ber_read_contextual_tag(s, 0, &length, TRUE);
	ber_read_octet_string_tag(s, &length);
	credssp->identity.DomainLength = (UINT32) length;
	credssp->identity.Domain = (UINT16*) malloc(length);
	CopyMemory(credssp->identity.Domain, Stream_Pointer(s), credssp->identity.DomainLength);
	Stream_Seek(s, credssp->identity.DomainLength);
	credssp->identity.DomainLength /= 2;

	 
	ber_read_contextual_tag(s, 1, &length, TRUE);
	ber_read_octet_string_tag(s, &length);
	credssp->identity.UserLength = (UINT32) length;
	credssp->identity.User = (UINT16*) malloc(length);
	CopyMemory(credssp->identity.User, Stream_Pointer(s), credssp->identity.UserLength);
	Stream_Seek(s, credssp->identity.UserLength);
	credssp->identity.UserLength /= 2;

	 
	ber_read_contextual_tag(s, 2, &length, TRUE);
	ber_read_octet_string_tag(s, &length);
	credssp->identity.PasswordLength = (UINT32) length;
	credssp->identity.Password = (UINT16*) malloc(length);
	CopyMemory(credssp->identity.Password, Stream_Pointer(s), credssp->identity.PasswordLength);
	Stream_Seek(s, credssp->identity.PasswordLength);
	credssp->identity.PasswordLength /= 2;

	credssp->identity.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
}