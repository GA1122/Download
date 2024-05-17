static inline void compat_release_entry(struct compat_arpt_entry *e)
{
	struct xt_entry_target *t;

	t = compat_arpt_get_target(e);
	module_put(t->u.kernel.target->me);
}
