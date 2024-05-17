int tracing_snapshot_cond_enable(struct trace_array *tr, void *cond_data,
				 cond_update_fn_t update)
{
	struct cond_snapshot *cond_snapshot;
	int ret = 0;

	cond_snapshot = kzalloc(sizeof(*cond_snapshot), GFP_KERNEL);
	if (!cond_snapshot)
		return -ENOMEM;

	cond_snapshot->cond_data = cond_data;
	cond_snapshot->update = update;

	mutex_lock(&trace_types_lock);

	ret = tracing_alloc_snapshot_instance(tr);
	if (ret)
		goto fail_unlock;

	if (tr->current_trace->use_max_tr) {
		ret = -EBUSY;
		goto fail_unlock;
	}

	 
	if (tr->cond_snapshot) {
		ret = -EBUSY;
		goto fail_unlock;
	}

	arch_spin_lock(&tr->max_lock);
	tr->cond_snapshot = cond_snapshot;
	arch_spin_unlock(&tr->max_lock);

	mutex_unlock(&trace_types_lock);

	return ret;

 fail_unlock:
	mutex_unlock(&trace_types_lock);
	kfree(cond_snapshot);
	return ret;
}
