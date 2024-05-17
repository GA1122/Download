static int set_permissions(struct ctl_table_header *head,
				  struct ctl_table *table)
{
	struct user_namespace *user_ns =
		container_of(head->set, struct user_namespace, set);
	int mode;

	 
	if (ns_capable(user_ns, CAP_SYS_RESOURCE))
		mode = (table->mode & S_IRWXU) >> 6;
	else
	 
		mode = table->mode & S_IROTH;
	return (mode << 6) | (mode << 3) | mode;
}