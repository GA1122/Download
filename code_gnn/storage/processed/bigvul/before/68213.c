static void llc_sk_init(struct sock *sk)
{
	struct llc_sock *llc = llc_sk(sk);

	llc->state    = LLC_CONN_STATE_ADM;
	llc->inc_cntr = llc->dec_cntr = 2;
	llc->dec_step = llc->connect_step = 1;

	setup_timer(&llc->ack_timer.timer, llc_conn_ack_tmr_cb,
			(unsigned long)sk);
	llc->ack_timer.expire	      = sysctl_llc2_ack_timeout;

	setup_timer(&llc->pf_cycle_timer.timer, llc_conn_pf_cycle_tmr_cb,
			(unsigned long)sk);
	llc->pf_cycle_timer.expire	   = sysctl_llc2_p_timeout;

	setup_timer(&llc->rej_sent_timer.timer, llc_conn_rej_tmr_cb,
			(unsigned long)sk);
	llc->rej_sent_timer.expire	   = sysctl_llc2_rej_timeout;

	setup_timer(&llc->busy_state_timer.timer, llc_conn_busy_tmr_cb,
			(unsigned long)sk);
	llc->busy_state_timer.expire	     = sysctl_llc2_busy_timeout;

	llc->n2 = 2;    
	llc->k  = 2;    
	llc->rw = 128;  
	skb_queue_head_init(&llc->pdu_unack_q);
	sk->sk_backlog_rcv = llc_backlog_rcv;
}