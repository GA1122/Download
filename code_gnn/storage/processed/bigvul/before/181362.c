 struct sock *inet_csk_clone_lock(const struct sock *sk,
 				 const struct request_sock *req,
 				 const gfp_t priority)
 {
 	struct sock *newsk = sk_clone_lock(sk, priority);
 
 	if (newsk) {
 		struct inet_connection_sock *newicsk = inet_csk(newsk);
 
 		newsk->sk_state = TCP_SYN_RECV;
 		newicsk->icsk_bind_hash = NULL;
 
 		inet_sk(newsk)->inet_dport = inet_rsk(req)->ir_rmt_port;
 		inet_sk(newsk)->inet_num = inet_rsk(req)->ir_num;
 		inet_sk(newsk)->inet_sport = htons(inet_rsk(req)->ir_num);
 		newsk->sk_write_space = sk_stream_write_space;
 
  		 
  		sock_reset_flag(newsk, SOCK_RCU_FREE);
  
// 		inet_sk(newsk)->mc_list = NULL;
// 
  		newsk->sk_mark = inet_rsk(req)->ir_mark;
  		atomic64_set(&newsk->sk_cookie,
  			     atomic64_read(&inet_rsk(req)->ir_cookie));
 
 		newicsk->icsk_retransmits = 0;
 		newicsk->icsk_backoff	  = 0;
 		newicsk->icsk_probes_out  = 0;
 
 		 
 		memset(&newicsk->icsk_accept_queue, 0, sizeof(newicsk->icsk_accept_queue));
 
 		security_inet_csk_clone(newsk, req);
 	}
 	return newsk;
 }