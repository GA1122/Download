sshpkt_get_bignum1(struct ssh *ssh, BIGNUM *v)
{
	return sshbuf_get_bignum1(ssh->state->incoming_packet, v);
}
