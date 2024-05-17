ssh_packet_is_rekeying(struct ssh *ssh)
{
	return compat20 &&
	    (ssh->state->rekeying || (ssh->kex != NULL && ssh->kex->done == 0));
}
