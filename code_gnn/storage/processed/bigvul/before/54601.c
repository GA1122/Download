static void scan_inflight(struct sock *x, void (*func)(struct unix_sock *),
			  struct sk_buff_head *hitlist)
{
	struct sk_buff *skb;
	struct sk_buff *next;

	spin_lock(&x->sk_receive_queue.lock);
	skb_queue_walk_safe(&x->sk_receive_queue, skb, next) {
		 
		if (UNIXCB(skb).fp) {
			bool hit = false;
			 
			int nfd = UNIXCB(skb).fp->count;
			struct file **fp = UNIXCB(skb).fp->fp;

			while (nfd--) {
				 
				struct sock *sk = unix_get_socket(*fp++);

				if (sk) {
					struct unix_sock *u = unix_sk(sk);

					 
					if (test_bit(UNIX_GC_CANDIDATE, &u->gc_flags)) {
						hit = true;

						func(u);
					}
				}
			}
			if (hit && hitlist != NULL) {
				__skb_unlink(skb, &x->sk_receive_queue);
				__skb_queue_tail(hitlist, skb);
			}
		}
	}
	spin_unlock(&x->sk_receive_queue.lock);
}
