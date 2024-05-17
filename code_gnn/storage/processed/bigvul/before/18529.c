static void tcp_illinois_acked(struct sock *sk, u32 pkts_acked, s32 rtt)
{
	struct illinois *ca = inet_csk_ca(sk);

	ca->acked = pkts_acked;

	 
	if (rtt < 0)
		return;

	 
	if (rtt > RTT_MAX)
		rtt = RTT_MAX;

	 
	if (ca->base_rtt > rtt)
		ca->base_rtt = rtt;

	 
	if (ca->max_rtt < rtt)
		ca->max_rtt = rtt;

	++ca->cnt_rtt;
	ca->sum_rtt += rtt;
}
