static int llc_backlog_rcv(struct sock *sk, struct sk_buff *skb)
{
	int rc = 0;
	struct llc_sock *llc = llc_sk(sk);

	if (likely(llc_backlog_type(skb) == LLC_PACKET)) {
		if (likely(llc->state > 1))  
			rc = llc_conn_rcv(sk, skb);
		else
			goto out_kfree_skb;
	} else if (llc_backlog_type(skb) == LLC_EVENT) {
		 
		if (likely(llc->state > 1))   
			rc = llc_conn_state_process(sk, skb);
		else
			goto out_kfree_skb;
	} else {
		printk(KERN_ERR "%s: invalid skb in backlog\n", __func__);
		goto out_kfree_skb;
	}
out:
	return rc;
out_kfree_skb:
	kfree_skb(skb);
	goto out;
}
