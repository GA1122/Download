void *ipc_rcu_alloc(int size)
{
	 
	struct ipc_rcu *out = ipc_alloc(sizeof(struct ipc_rcu) + size);
	if (unlikely(!out))
		return NULL;
	atomic_set(&out->refcount, 1);
	return out + 1;
}
