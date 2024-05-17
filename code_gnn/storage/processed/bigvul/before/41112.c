static inline void tcp_ack_snd_check(struct sock *sk)
{
	if (!inet_csk_ack_scheduled(sk)) {
		 
		return;
	}
	__tcp_ack_snd_check(sk, 1);
}
