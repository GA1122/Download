struct dentry *tracing_init_dentry(void)
{
	struct trace_array *tr = &global_trace;

	 
	if (tr->dir)
		return NULL;

	if (WARN_ON(!tracefs_initialized()) ||
		(IS_ENABLED(CONFIG_DEBUG_FS) &&
		 WARN_ON(!debugfs_initialized())))
		return ERR_PTR(-ENODEV);

	 
	tr->dir = debugfs_create_automount("tracing", NULL,
					   trace_automount, NULL);
	if (!tr->dir) {
		pr_warn_once("Could not create debugfs directory 'tracing'\n");
		return ERR_PTR(-ENOMEM);
	}

	return NULL;
}
