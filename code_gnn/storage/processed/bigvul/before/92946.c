mcs_out_domain_params(STREAM s, int max_channels, int max_users, int max_tokens, int max_pdusize)
{
	ber_out_header(s, MCS_TAG_DOMAIN_PARAMS, 32);
	ber_out_integer(s, max_channels);
	ber_out_integer(s, max_users);
	ber_out_integer(s, max_tokens);
	ber_out_integer(s, 1);	 
	ber_out_integer(s, 0);	 
	ber_out_integer(s, 1);	 
	ber_out_integer(s, max_pdusize);
	ber_out_integer(s, 2);	 
}
