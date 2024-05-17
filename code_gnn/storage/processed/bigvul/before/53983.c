int ndp_open(struct ndp **p_ndp)
{
	struct ndp *ndp;
	const char *env;
	int err;

	ndp = myzalloc(sizeof(*ndp));
	if (!ndp)
		return -ENOMEM;
	ndp->log_fn = log_stderr;
	ndp->log_priority = LOG_ERR;
	 
	env = getenv("NDP_LOG");
	if (env != NULL)
		ndp_set_log_priority(ndp, log_priority(env));

	dbg(ndp, "ndp context %p created.", ndp);
	dbg(ndp, "log_priority=%d", ndp->log_priority);

	list_init(&ndp->msgrcv_handler_list);
	err = ndp_sock_open(ndp);
	if (err)
		goto free_ndp;

	*p_ndp = ndp;
	return 0;
free_ndp:
	free(ndp);
	return err;
}