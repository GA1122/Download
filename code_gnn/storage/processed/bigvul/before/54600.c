static void scan_children(struct sock *x, void (*func)(struct unix_sock *),
			  struct sk_buff_head *hitlist)
{
	if (x->sk_state != TCP_LISTEN) {
		scan_inflight(x, func, hitlist);
	} else {
		struct sk_buff *skb;
		struct sk_buff *next;
		struct unix_sock *u;
		LIST_HEAD(embryos);

		 
		spin_lock(&x->sk_receive_queue.lock);
		skb_queue_walk_safe(&x->sk_receive_queue, skb, next) {
			u = unix_sk(skb->sk);

			 
			BUG_ON(!list_empty(&u->link));
			list_add_tail(&u->link, &embryos);
		}
		spin_unlock(&x->sk_receive_queue.lock);

		while (!list_empty(&embryos)) {
			u = list_entry(embryos.next, struct unix_sock, link);
			scan_inflight(&u->sk, func, hitlist);
			list_del_init(&u->link);
		}
	}
}