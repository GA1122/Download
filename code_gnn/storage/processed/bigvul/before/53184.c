static int llc_wait_data(struct sock *sk, long timeo)
{
	int rc;

	while (1) {
		 
		rc = sock_error(sk);
		if (rc)
			break;
		rc = 0;
		if (sk->sk_shutdown & RCV_SHUTDOWN)
			break;
		rc = -EAGAIN;
		if (!timeo)
			break;
		rc = sock_intr_errno(timeo);
		if (signal_pending(current))
			break;
		rc = 0;
		if (sk_wait_data(sk, &timeo, NULL))
			break;
	}
	return rc;
}