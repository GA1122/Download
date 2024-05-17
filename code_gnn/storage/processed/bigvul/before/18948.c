static void raw_close(struct sock *sk, long timeout)
{
	 
	ip_ra_control(sk, 0, NULL);

	sk_common_release(sk);
}
