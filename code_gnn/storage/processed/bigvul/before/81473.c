static struct dentry *tracing_get_dentry(struct trace_array *tr)
{
	if (WARN_ON(!tr->dir))
		return ERR_PTR(-ENODEV);

	 
	if (tr->flags & TRACE_ARRAY_FL_GLOBAL)
		return NULL;

	 
	return tr->dir;
}
