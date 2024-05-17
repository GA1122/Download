input_userauth_ext_info(int type, u_int32_t seqnr, void *ctxt)
{
	return kex_input_ext_info(type, seqnr, active_state);
}
