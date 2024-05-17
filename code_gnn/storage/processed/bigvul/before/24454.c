void sysctl_head_put(struct ctl_table_header *head)
{
	spin_lock(&sysctl_lock);
	if (!--head->count)
		call_rcu(&head->rcu, free_head);
	spin_unlock(&sysctl_lock);
}
