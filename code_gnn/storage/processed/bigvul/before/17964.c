kex_protocol_error(int type, u_int32_t seq, void *ctxt)
{
	struct ssh *ssh = active_state;  
	int r;

	error("kex protocol error: type %d seq %u", type, seq);
	if ((r = sshpkt_start(ssh, SSH2_MSG_UNIMPLEMENTED)) != 0 ||
	    (r = sshpkt_put_u32(ssh, seq)) != 0 ||
	    (r = sshpkt_send(ssh)) != 0)
		return r;
	return 0;
}