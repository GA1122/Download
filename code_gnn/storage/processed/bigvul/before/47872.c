struct sock *sk_clone_lock(const struct sock *sk, const gfp_t priority)
{
	struct sock *newsk;
	bool is_charged = true;

	newsk = sk_prot_alloc(sk->sk_prot, priority, sk->sk_family);
	if (newsk != NULL) {
		struct sk_filter *filter;

		sock_copy(newsk, sk);

		 
		if (likely(newsk->sk_net_refcnt))
			get_net(sock_net(newsk));
		sk_node_init(&newsk->sk_node);
		sock_lock_init(newsk);
		bh_lock_sock(newsk);
		newsk->sk_backlog.head	= newsk->sk_backlog.tail = NULL;
		newsk->sk_backlog.len = 0;

		atomic_set(&newsk->sk_rmem_alloc, 0);
		 
		atomic_set(&newsk->sk_wmem_alloc, 1);
		atomic_set(&newsk->sk_omem_alloc, 0);
		skb_queue_head_init(&newsk->sk_receive_queue);
		skb_queue_head_init(&newsk->sk_write_queue);

		rwlock_init(&newsk->sk_callback_lock);
		lockdep_set_class_and_name(&newsk->sk_callback_lock,
				af_callback_keys + newsk->sk_family,
				af_family_clock_key_strings[newsk->sk_family]);

		newsk->sk_dst_cache	= NULL;
		newsk->sk_wmem_queued	= 0;
		newsk->sk_forward_alloc = 0;
		atomic_set(&newsk->sk_drops, 0);
		newsk->sk_send_head	= NULL;
		newsk->sk_userlocks	= sk->sk_userlocks & ~SOCK_BINDPORT_LOCK;

		sock_reset_flag(newsk, SOCK_DONE);
		skb_queue_head_init(&newsk->sk_error_queue);

		filter = rcu_dereference_protected(newsk->sk_filter, 1);
		if (filter != NULL)
			 
			is_charged = sk_filter_charge(newsk, filter);

		if (unlikely(!is_charged || xfrm_sk_clone_policy(newsk, sk))) {
			 
			newsk->sk_destruct = NULL;
			bh_unlock_sock(newsk);
			sk_free(newsk);
			newsk = NULL;
			goto out;
		}
		RCU_INIT_POINTER(newsk->sk_reuseport_cb, NULL);

		newsk->sk_err	   = 0;
		newsk->sk_err_soft = 0;
		newsk->sk_priority = 0;
		newsk->sk_incoming_cpu = raw_smp_processor_id();
		atomic64_set(&newsk->sk_cookie, 0);

		mem_cgroup_sk_alloc(newsk);
		cgroup_sk_alloc(&newsk->sk_cgrp_data);

		 
		smp_wmb();
		atomic_set(&newsk->sk_refcnt, 2);

		 
		sk_refcnt_debug_inc(newsk);
		sk_set_socket(newsk, NULL);
		newsk->sk_wq = NULL;

		if (newsk->sk_prot->sockets_allocated)
			sk_sockets_allocated_inc(newsk);

		if (sock_needs_netstamp(sk) &&
		    newsk->sk_flags & SK_FLAGS_TIMESTAMP)
			net_enable_timestamp();
	}
out:
	return newsk;
}