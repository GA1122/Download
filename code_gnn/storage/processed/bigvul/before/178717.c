 void inet_sock_destruct(struct sock *sk)
 {
 	struct inet_sock *inet = inet_sk(sk);
 
 	__skb_queue_purge(&sk->sk_receive_queue);
 	__skb_queue_purge(&sk->sk_error_queue);
 
 	sk_mem_reclaim(sk);
 
 	if (sk->sk_type == SOCK_STREAM && sk->sk_state != TCP_CLOSE) {
 		pr_err("Attempt to release TCP socket in state %d %p\n",
 		       sk->sk_state, sk);
 		return;
 	}
 	if (!sock_flag(sk, SOCK_DEAD)) {
 		pr_err("Attempt to release alive inet socket %p\n", sk);
 		return;
 	}
 
 	WARN_ON(atomic_read(&sk->sk_rmem_alloc));
 	WARN_ON(atomic_read(&sk->sk_wmem_alloc));
  	WARN_ON(sk->sk_wmem_queued);
  	WARN_ON(sk->sk_forward_alloc);
  
	kfree(inet->opt);
// 	kfree(rcu_dereference_protected(inet->inet_opt, 1));
  	dst_release(rcu_dereference_check(sk->sk_dst_cache, 1));
  	sk_refcnt_debug_dec(sk);
  }