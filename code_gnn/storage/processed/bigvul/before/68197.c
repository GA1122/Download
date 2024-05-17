void llc_conn_send_pdu(struct sock *sk, struct sk_buff *skb)
{
	 
	skb_queue_tail(&sk->sk_write_queue, skb);
	llc_conn_send_pdus(sk);
}
