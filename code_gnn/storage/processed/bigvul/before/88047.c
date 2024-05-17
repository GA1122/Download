static int genl_lock_start(struct netlink_callback *cb)
{
	 
	const struct genl_ops *ops = cb->data;
	int rc = 0;

	if (ops->start) {
		genl_lock();
		rc = ops->start(cb);
		genl_unlock();
	}
	return rc;
}