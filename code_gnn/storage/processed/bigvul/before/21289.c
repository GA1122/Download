struct mempolicy *__mpol_dup(struct mempolicy *old)
{
	struct mempolicy *new = kmem_cache_alloc(policy_cache, GFP_KERNEL);

	if (!new)
		return ERR_PTR(-ENOMEM);

	 
	if (old == current->mempolicy) {
		task_lock(current);
		*new = *old;
		task_unlock(current);
	} else
		*new = *old;

	rcu_read_lock();
	if (current_cpuset_is_being_rebound()) {
		nodemask_t mems = cpuset_mems_allowed(current);
		if (new->flags & MPOL_F_REBINDING)
			mpol_rebind_policy(new, &mems, MPOL_REBIND_STEP2);
		else
			mpol_rebind_policy(new, &mems, MPOL_REBIND_ONCE);
	}
	rcu_read_unlock();
	atomic_set(&new->refcnt, 1);
	return new;
}