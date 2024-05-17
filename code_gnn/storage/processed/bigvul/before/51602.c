static void tcp_rtt_estimator(struct sock *sk, long mrtt_us)
{
	struct tcp_sock *tp = tcp_sk(sk);
	long m = mrtt_us;  
	u32 srtt = tp->srtt_us;

	 
	if (srtt != 0) {
		m -= (srtt >> 3);	 
		srtt += m;		 
		if (m < 0) {
			m = -m;		 
			m -= (tp->mdev_us >> 2);    
			 
			if (m > 0)
				m >>= 3;
		} else {
			m -= (tp->mdev_us >> 2);    
		}
		tp->mdev_us += m;		 
		if (tp->mdev_us > tp->mdev_max_us) {
			tp->mdev_max_us = tp->mdev_us;
			if (tp->mdev_max_us > tp->rttvar_us)
				tp->rttvar_us = tp->mdev_max_us;
		}
		if (after(tp->snd_una, tp->rtt_seq)) {
			if (tp->mdev_max_us < tp->rttvar_us)
				tp->rttvar_us -= (tp->rttvar_us - tp->mdev_max_us) >> 2;
			tp->rtt_seq = tp->snd_nxt;
			tp->mdev_max_us = tcp_rto_min_us(sk);
		}
	} else {
		 
		srtt = m << 3;		 
		tp->mdev_us = m << 1;	 
		tp->rttvar_us = max(tp->mdev_us, tcp_rto_min_us(sk));
		tp->mdev_max_us = tp->rttvar_us;
		tp->rtt_seq = tp->snd_nxt;
	}
	tp->srtt_us = max(1U, srtt);
}
