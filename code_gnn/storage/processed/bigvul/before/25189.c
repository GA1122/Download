static int slow_chain_length(const struct rtable *head)
{
	int length = 0;
	const struct rtable *rth = head;

	while (rth) {
		length += has_noalias(head, rth);
		rth = rcu_dereference_protected(rth->dst.rt_next, 1);
	}
	return length >> FRACT_BITS;
}
