g_token_size(gss_OID_const mech, unsigned int body_size)
{
	int hdrsize;

	 
	hdrsize = 1 + gssint_der_length_size(mech->length) + mech->length;

	 
	hdrsize += 1 + gssint_der_length_size(body_size + hdrsize);

	return (hdrsize + body_size);
}
