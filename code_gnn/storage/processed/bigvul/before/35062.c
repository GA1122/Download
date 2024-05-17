void sctp_wait_for_close(struct sock *sk, long timeout)
{
	DEFINE_WAIT(wait);

	do {
		prepare_to_wait(sk->sk_sleep, &wait, TASK_INTERRUPTIBLE);
		if (list_empty(&sctp_sk(sk)->ep->asocs))
			break;
		sctp_release_sock(sk);
		timeout = schedule_timeout(timeout);
		sctp_lock_sock(sk);
	} while (!signal_pending(current) && timeout);

 	finish_wait(sk->sk_sleep, &wait);
 }