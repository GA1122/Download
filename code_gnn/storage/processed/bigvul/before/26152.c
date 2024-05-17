void perf_pmu_unregister(struct pmu *pmu)
{
	mutex_lock(&pmus_lock);
	list_del_rcu(&pmu->entry);
	mutex_unlock(&pmus_lock);

	 
	synchronize_srcu(&pmus_srcu);
	synchronize_rcu();

	free_percpu(pmu->pmu_disable_count);
	if (pmu->type >= PERF_TYPE_MAX)
		idr_remove(&pmu_idr, pmu->type);
	device_del(pmu->dev);
	put_device(pmu->dev);
	free_pmu_context(pmu);
}