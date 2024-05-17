static void free_pmu_context(struct pmu *pmu)
{
	struct pmu *i;

	mutex_lock(&pmus_lock);
	 
	list_for_each_entry(i, &pmus, entry) {
		if (i->pmu_cpu_context == pmu->pmu_cpu_context) {
			update_pmu_context(i, pmu);
			goto out;
		}
	}

	free_percpu(pmu->pmu_cpu_context);
out:
	mutex_unlock(&pmus_lock);
}