void unregister_sysctl_table(struct ctl_table_header * header)
{
	might_sleep();

	if (header == NULL)
		return;

	spin_lock(&sysctl_lock);
	start_unregistering(header);
	if (!--header->parent->count) {
		WARN_ON(1);
		call_rcu(&header->parent->rcu, free_head);
	}
	if (!--header->count)
		call_rcu(&header->rcu, free_head);
	spin_unlock(&sysctl_lock);
}
