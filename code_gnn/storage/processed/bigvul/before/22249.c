void rose_requeue_frames(struct sock *sk)
{
	struct sk_buff *skb, *skb_prev = NULL;

	 
	while ((skb = skb_dequeue(&rose_sk(sk)->ack_queue)) != NULL) {
		if (skb_prev == NULL)
			skb_queue_head(&sk->sk_write_queue, skb);
		else
			skb_append(skb_prev, skb, &sk->sk_write_queue);
		skb_prev = skb;
	}
}
