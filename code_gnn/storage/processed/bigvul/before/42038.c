struct kern_ipc_perm *ipc_lock(struct ipc_ids *ids, int id)
{
	struct kern_ipc_perm *out;

	rcu_read_lock();
	out = ipc_obtain_object_idr(ids, id);
	if (IS_ERR(out))
		goto err;

	spin_lock(&out->lock);

	 
	if (ipc_valid_object(out))
		return out;

	spin_unlock(&out->lock);
	out = ERR_PTR(-EIDRM);
err:
	rcu_read_unlock();
	return out;
}
