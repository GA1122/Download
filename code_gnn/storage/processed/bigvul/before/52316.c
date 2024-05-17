void ipt_unregister_table(struct net *net, struct xt_table *table,
			  const struct nf_hook_ops *ops)
{
	nf_unregister_net_hooks(net, ops, hweight32(table->valid_hooks));
	__ipt_unregister_table(net, table);
}