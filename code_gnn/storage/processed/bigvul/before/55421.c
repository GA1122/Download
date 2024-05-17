static void tcp_update_rtt_min(struct sock *sk, u32 rtt_us)
{
	const u32 now = tcp_time_stamp, wlen = sysctl_tcp_min_rtt_wlen * HZ;
	struct rtt_meas *m = tcp_sk(sk)->rtt_min;
	struct rtt_meas rttm = { .rtt = (rtt_us ? : 1), .ts = now };
	u32 elapsed;

	 
	if (unlikely(rttm.rtt <= m[0].rtt))
		m[0] = m[1] = m[2] = rttm;
	else if (rttm.rtt <= m[1].rtt)
		m[1] = m[2] = rttm;
	else if (rttm.rtt <= m[2].rtt)
		m[2] = rttm;

	elapsed = now - m[0].ts;
	if (unlikely(elapsed > wlen)) {
		 
		m[0] = m[1];
		m[1] = m[2];
		m[2] = rttm;
		if (now - m[0].ts > wlen) {
			m[0] = m[1];
			m[1] = rttm;
			if (now - m[0].ts > wlen)
				m[0] = rttm;
		}
	} else if (m[1].ts == m[0].ts && elapsed > wlen / 4) {
		 
		m[2] = m[1] = rttm;
	} else if (m[2].ts == m[1].ts && elapsed > wlen / 2) {
		 
		m[2] = rttm;
	}
}
