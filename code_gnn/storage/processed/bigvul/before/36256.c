void sctp_assoc_rwnd_decrease(struct sctp_association *asoc, unsigned int len)
{
	int rx_count;
	int over = 0;

	if (unlikely(!asoc->rwnd || asoc->rwnd_over))
		pr_debug("%s: association:%p has asoc->rwnd:%u, "
			 "asoc->rwnd_over:%u!\n", __func__, asoc,
			 asoc->rwnd, asoc->rwnd_over);

	if (asoc->ep->rcvbuf_policy)
		rx_count = atomic_read(&asoc->rmem_alloc);
	else
		rx_count = atomic_read(&asoc->base.sk->sk_rmem_alloc);

	 
	if (rx_count >= asoc->base.sk->sk_rcvbuf)
		over = 1;

	if (asoc->rwnd >= len) {
		asoc->rwnd -= len;
		if (over) {
			asoc->rwnd_press += asoc->rwnd;
			asoc->rwnd = 0;
		}
	} else {
		asoc->rwnd_over = len - asoc->rwnd;
		asoc->rwnd = 0;
	}

	pr_debug("%s: asoc:%p rwnd decreased by %d to (%u, %u, %u)\n",
		 __func__, asoc, len, asoc->rwnd, asoc->rwnd_over,
		 asoc->rwnd_press);
}
