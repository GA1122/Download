server_input_keep_alive(int type, u_int32_t seq, void *ctxt)
{
	debug("Got %d/%u for keepalive", type, seq);
	 
	packet_set_alive_timeouts(0);
	return 0;
}
