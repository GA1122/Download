static void start_unregistering(struct ctl_table_header *p)
{
	 
	if (unlikely(p->used)) {
		struct completion wait;
		init_completion(&wait);
		p->unregistering = &wait;
		spin_unlock(&sysctl_lock);
		wait_for_completion(&wait);
		spin_lock(&sysctl_lock);
	} else {
		 
		p->unregistering = ERR_PTR(-EINVAL);
	}
	 
	list_del_init(&p->ctl_entry);
}
