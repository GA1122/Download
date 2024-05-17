static void iucv_process_message(struct sock *sk, struct sk_buff *skb,
				 struct iucv_path *path,
				 struct iucv_message *msg)
{
	int rc;
	unsigned int len;

	len = iucv_msg_length(msg);

	 
	 
	memcpy(CB_TRGCLS(skb), &msg->class, CB_TRGCLS_LEN);

	 
	if ((msg->flags & IUCV_IPRMDATA) && len > 7) {
		if (memcmp(msg->rmmsg, iprm_shutdown, 8) == 0) {
			skb->data = NULL;
			skb->len = 0;
		}
	} else {
		rc = pr_iucv->message_receive(path, msg,
					      msg->flags & IUCV_IPRMDATA,
					      skb->data, len, NULL);
		if (rc) {
			kfree_skb(skb);
			return;
		}
		 
		if (sk->sk_type == SOCK_STREAM &&
		    skb->truesize >= sk->sk_rcvbuf / 4) {
			rc = iucv_fragment_skb(sk, skb, len);
			kfree_skb(skb);
			skb = NULL;
			if (rc) {
				pr_iucv->path_sever(path, NULL);
				return;
			}
			skb = skb_dequeue(&iucv_sk(sk)->backlog_skb_q);
		} else {
			skb_reset_transport_header(skb);
			skb_reset_network_header(skb);
			skb->len = len;
		}
	}

	if (sock_queue_rcv_skb(sk, skb))
		skb_queue_head(&iucv_sk(sk)->backlog_skb_q, skb);
}