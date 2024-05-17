bool lock_sock_fast(struct sock *sk)
{
	might_sleep();
	spin_lock_bh(&sk->sk_lock.slock);

	if (!sk->sk_lock.owned)
		 
		return false;

	__lock_sock(sk);
	sk->sk_lock.owned = 1;
	spin_unlock(&sk->sk_lock.slock);
	 
	mutex_acquire(&sk->sk_lock.dep_map, 0, 0, _RET_IP_);
	local_bh_enable();
	return true;
}
