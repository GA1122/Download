static void __tcp_ack_snd_check(struct sock *sk, int ofo_possible)
{
	struct tcp_sock *tp = tcp_sk(sk);

	     
	if (((tp->rcv_nxt - tp->rcv_wup) > inet_csk(sk)->icsk_ack.rcv_mss &&
	      
	     __tcp_select_window(sk) >= tp->rcv_wnd) ||
	     
	    tcp_in_quickack_mode(sk) ||
	     
	    (ofo_possible && skb_peek(&tp->out_of_order_queue))) {
		 
		tcp_send_ack(sk);
	} else {
		 
		tcp_send_delayed_ack(sk);
	}
}
