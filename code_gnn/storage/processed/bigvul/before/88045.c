static int genl_lock_done(struct netlink_callback *cb)
{
	 
	const struct genl_ops *ops = cb->data;
	int rc = 0;

	if (ops->done) {
		genl_lock();
		rc = ops->done(cb);
		genl_unlock();
	}
	return rc;
}
