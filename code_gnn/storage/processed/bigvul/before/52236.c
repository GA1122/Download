static inline int check_entry(const struct arpt_entry *e)
{
	const struct xt_entry_target *t;

	if (!arp_checkentry(&e->arp))
		return -EINVAL;

	if (e->target_offset + sizeof(struct xt_entry_target) > e->next_offset)
		return -EINVAL;

	t = arpt_get_target_c(e);
	if (e->target_offset + t->u.target_size > e->next_offset)
		return -EINVAL;

	return 0;
}
