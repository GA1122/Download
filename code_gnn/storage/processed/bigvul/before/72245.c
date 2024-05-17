input_userauth_error(int type, u_int32_t seq, void *ctxt)
{
	fatal("input_userauth_error: bad message during authentication: "
	    "type %d", type);
	return 0;
}
