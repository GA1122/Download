static void add_ftrace_ops(struct ftrace_ops **list, struct ftrace_ops *ops)
{
	ops->next = *list;
	 
	rcu_assign_pointer(*list, ops);
}
