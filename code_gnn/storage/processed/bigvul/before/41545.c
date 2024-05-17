static long inet_wait_for_connect(struct sock *sk, long timeo, int writebias)
{
	DEFINE_WAIT(wait);

	prepare_to_wait(sk_sleep(sk), &wait, TASK_INTERRUPTIBLE);
	sk->sk_write_pending += writebias;

	 
	while ((1 << sk->sk_state) & (TCPF_SYN_SENT | TCPF_SYN_RECV)) {
		release_sock(sk);
		timeo = schedule_timeout(timeo);
		lock_sock(sk);
		if (signal_pending(current) || !timeo)
			break;
		prepare_to_wait(sk_sleep(sk), &wait, TASK_INTERRUPTIBLE);
	}
	finish_wait(sk_sleep(sk), &wait);
	sk->sk_write_pending -= writebias;
	return timeo;
}