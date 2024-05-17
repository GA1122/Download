void rose_frames_acked(struct sock *sk, unsigned short nr)
{
	struct sk_buff *skb;
	struct rose_sock *rose = rose_sk(sk);

	 
	if (rose->va != nr) {
		while (skb_peek(&rose->ack_queue) != NULL && rose->va != nr) {
			skb = skb_dequeue(&rose->ack_queue);
			kfree_skb(skb);
			rose->va = (rose->va + 1) % ROSE_MODULUS;
		}
	}
}
