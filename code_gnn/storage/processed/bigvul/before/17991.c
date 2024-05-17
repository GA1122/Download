ssh_packet_set_rekey_limits(struct ssh *ssh, u_int64_t bytes, time_t seconds)
{
	debug3("rekey after %llu bytes, %d seconds", (unsigned long long)bytes,
	    (int)seconds);
	ssh->state->rekey_limit = bytes;
	ssh->state->rekey_interval = seconds;
}
