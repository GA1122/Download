static inline void cleanup_entry(struct arpt_entry *e)
{
	struct xt_tgdtor_param par;
	struct xt_entry_target *t;

	t = arpt_get_target(e);
	par.target   = t->u.kernel.target;
	par.targinfo = t->data;
	par.family   = NFPROTO_ARP;
	if (par.target->destroy != NULL)
		par.target->destroy(&par);
	module_put(par.target->me);
	xt_percpu_counter_free(&e->counters);
}
