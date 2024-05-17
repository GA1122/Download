static int has_noalias(const struct rtable *head, const struct rtable *rth)
{
	const struct rtable *aux = head;

	while (aux != rth) {
		if (compare_hash_inputs(aux, rth))
			return 0;
		aux = rcu_dereference_protected(aux->dst.rt_next, 1);
	}
	return ONE;
}
