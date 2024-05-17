static struct sk_buff *tun_alloc_skb(struct tun_struct *tun,
				     size_t prepad, size_t len,
				     size_t linear, int noblock)
{
	struct sock *sk = tun->socket.sk;
	struct sk_buff *skb;
	int err;

	sock_update_classid(sk);

	 
	if (prepad + len < PAGE_SIZE || !linear)
		linear = len;

	skb = sock_alloc_send_pskb(sk, prepad + linear, len - linear, noblock,
				   &err);
	if (!skb)
		return ERR_PTR(err);

	skb_reserve(skb, prepad);
	skb_put(skb, linear);
	skb->data_len = len - linear;
	skb->len += len - linear;

	return skb;
}