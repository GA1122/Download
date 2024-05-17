bool cookie_check_timestamp(struct tcp_options_received *tcp_opt, bool *ecn_ok)
{
	 
	u32 options = tcp_opt->rcv_tsecr & TSMASK;

	if (!tcp_opt->saw_tstamp)  {
		tcp_clear_options(tcp_opt);
		return true;
	}

	if (!sysctl_tcp_timestamps)
		return false;

	tcp_opt->sack_ok = (options >> 4) & 0x1;
	*ecn_ok = (options >> 5) & 1;
	if (*ecn_ok && !sysctl_tcp_ecn)
		return false;

	if (tcp_opt->sack_ok && !sysctl_tcp_sack)
		return false;

	if ((options & 0xf) == 0xf)
		return true;  

	tcp_opt->wscale_ok = 1;
	tcp_opt->snd_wscale = options & 0xf;
	return sysctl_tcp_window_scaling != 0;
}