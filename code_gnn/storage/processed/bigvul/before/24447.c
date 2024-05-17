void setup_sysctl_set(struct ctl_table_set *p,
	struct ctl_table_set *parent,
	int (*is_seen)(struct ctl_table_set *))
{
	INIT_LIST_HEAD(&p->list);
	p->parent = parent ? parent : &sysctl_table_root.default_set;
	p->is_seen = is_seen;
}
