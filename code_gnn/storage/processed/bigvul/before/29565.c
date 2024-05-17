struct kern_ipc_perm *ipc_lock(struct ipc_ids *ids, int id)
{
	struct kern_ipc_perm *out;

	rcu_read_lock();
	out = ipc_obtain_object(ids, id);
	if (IS_ERR(out))
		goto err1;

	spin_lock(&out->lock);

	 
	if (!out->deleted)
		return out;

	spin_unlock(&out->lock);
	out = ERR_PTR(-EINVAL);
err1:
	rcu_read_unlock();
	return out;
}
