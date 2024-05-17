int tcp_cookie_generator(u32 *bakery)
{
	unsigned long jiffy = jiffies;

	if (unlikely(time_after_eq(jiffy, tcp_secret_generating->expires))) {
		spin_lock_bh(&tcp_secret_locker);
		if (!time_after_eq(jiffy, tcp_secret_generating->expires)) {
			 
			memcpy(bakery,
			       &tcp_secret_generating->secrets[0],
			       COOKIE_WORKSPACE_WORDS);
		} else {
			 
			get_random_bytes(bakery, COOKIE_WORKSPACE_WORDS);

			 
			if (unlikely(tcp_secret_primary->expires ==
				     tcp_secret_secondary->expires)) {
				struct timespec tv;

				getnstimeofday(&tv);
				bakery[COOKIE_DIGEST_WORDS+0] ^=
					(u32)tv.tv_nsec;

				tcp_secret_secondary->expires = jiffy
					+ TCP_SECRET_1MSL
					+ (0x0f & tcp_cookie_work(bakery, 0));
			} else {
				tcp_secret_secondary->expires = jiffy
					+ TCP_SECRET_LIFE
					+ (0xff & tcp_cookie_work(bakery, 1));
				tcp_secret_primary->expires = jiffy
					+ TCP_SECRET_2MSL
					+ (0x1f & tcp_cookie_work(bakery, 2));
			}
			memcpy(&tcp_secret_secondary->secrets[0],
			       bakery, COOKIE_WORKSPACE_WORDS);

			rcu_assign_pointer(tcp_secret_generating,
					   tcp_secret_secondary);
			rcu_assign_pointer(tcp_secret_retiring,
					   tcp_secret_primary);
			 
		}
		spin_unlock_bh(&tcp_secret_locker);
	} else {
		rcu_read_lock_bh();
		memcpy(bakery,
		       &rcu_dereference(tcp_secret_generating)->secrets[0],
		       COOKIE_WORKSPACE_WORDS);
		rcu_read_unlock_bh();
	}
	return 0;
}
