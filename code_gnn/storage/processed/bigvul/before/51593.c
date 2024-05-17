void tcp_rcv_space_adjust(struct sock *sk)
{
	struct tcp_sock *tp = tcp_sk(sk);
	int time;
	int copied;

	time = tcp_time_stamp - tp->rcvq_space.time;
	if (time < (tp->rcv_rtt_est.rtt >> 3) || tp->rcv_rtt_est.rtt == 0)
		return;

	 
	copied = tp->copied_seq - tp->rcvq_space.seq;
	if (copied <= tp->rcvq_space.space)
		goto new_measure;

	 

	if (sysctl_tcp_moderate_rcvbuf &&
	    !(sk->sk_userlocks & SOCK_RCVBUF_LOCK)) {
		int rcvwin, rcvmem, rcvbuf;

		 
		rcvwin = (copied << 1) + 16 * tp->advmss;

		 
		if (copied >=
		    tp->rcvq_space.space + (tp->rcvq_space.space >> 2)) {
			if (copied >=
			    tp->rcvq_space.space + (tp->rcvq_space.space >> 1))
				rcvwin <<= 1;
			else
				rcvwin += (rcvwin >> 1);
		}

		rcvmem = SKB_TRUESIZE(tp->advmss + MAX_TCP_HEADER);
		while (tcp_win_from_space(rcvmem) < tp->advmss)
			rcvmem += 128;

		rcvbuf = min(rcvwin / tp->advmss * rcvmem, sysctl_tcp_rmem[2]);
		if (rcvbuf > sk->sk_rcvbuf) {
			sk->sk_rcvbuf = rcvbuf;

			 
			tp->window_clamp = rcvwin;
		}
	}
	tp->rcvq_space.space = copied;

new_measure:
	tp->rcvq_space.seq = tp->copied_seq;
	tp->rcvq_space.time = tcp_time_stamp;
}
