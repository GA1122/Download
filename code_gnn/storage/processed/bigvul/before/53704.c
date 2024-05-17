static int raw6_getfrag(void *from, char *to, int offset, int len, int odd,
		       struct sk_buff *skb)
{
	struct raw6_frag_vec *rfv = from;

	if (offset < rfv->hlen) {
		int copy = min(rfv->hlen - offset, len);

		if (skb->ip_summed == CHECKSUM_PARTIAL)
			memcpy(to, rfv->c + offset, copy);
		else
			skb->csum = csum_block_add(
				skb->csum,
				csum_partial_copy_nocheck(rfv->c + offset,
							  to, copy, 0),
				odd);

		odd = 0;
		offset += copy;
		to += copy;
		len -= copy;

		if (!len)
			return 0;
	}

	offset -= rfv->hlen;

	return ip_generic_getfrag(rfv->msg, to, offset, len, odd, skb);
}