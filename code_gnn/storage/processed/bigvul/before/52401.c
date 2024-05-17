int xt_check_target(struct xt_tgchk_param *par,
		    unsigned int size, u_int8_t proto, bool inv_proto)
{
	int ret;

	if (XT_ALIGN(par->target->targetsize) != size) {
		pr_err("%s_tables: %s.%u target: invalid size "
		       "%u (kernel) != (user) %u\n",
		       xt_prefix[par->family], par->target->name,
		       par->target->revision,
		       XT_ALIGN(par->target->targetsize), size);
		return -EINVAL;
	}
	if (par->target->table != NULL &&
	    strcmp(par->target->table, par->table) != 0) {
		pr_err("%s_tables: %s target: only valid in %s table, not %s\n",
		       xt_prefix[par->family], par->target->name,
		       par->target->table, par->table);
		return -EINVAL;
	}
	if (par->target->hooks && (par->hook_mask & ~par->target->hooks) != 0) {
		char used[64], allow[64];

		pr_err("%s_tables: %s target: used from hooks %s, but only "
		       "usable from %s\n",
		       xt_prefix[par->family], par->target->name,
		       textify_hooks(used, sizeof(used), par->hook_mask,
		                     par->family),
		       textify_hooks(allow, sizeof(allow), par->target->hooks,
		                     par->family));
		return -EINVAL;
	}
	if (par->target->proto && (par->target->proto != proto || inv_proto)) {
		pr_err("%s_tables: %s target: only valid for protocol %u\n",
		       xt_prefix[par->family], par->target->name,
		       par->target->proto);
		return -EINVAL;
	}
	if (par->target->checkentry != NULL) {
		ret = par->target->checkentry(par);
		if (ret < 0)
			return ret;
		else if (ret > 0)
			 
			return -EIO;
	}
	return 0;
}