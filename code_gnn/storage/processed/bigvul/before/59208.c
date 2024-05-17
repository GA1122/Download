static void raw_close(struct sock *sk, long timeout)
{
	 
	rtnl_lock();
	ip_ra_control(sk, 0, NULL);
	rtnl_unlock();

	sk_common_release(sk);
}
