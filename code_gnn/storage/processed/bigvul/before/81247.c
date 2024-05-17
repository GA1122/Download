add_trace_export(struct trace_export **list, struct trace_export *export)
{
	rcu_assign_pointer(export->next, *list);
	 
	rcu_assign_pointer(*list, export);
}
