nvmet_fc_queue_to_cpu(struct nvmet_fc_tgtport *tgtport, int qid)
{
	int cpu, idx, cnt;

	if (tgtport->ops->max_hw_queues == 1)
		return WORK_CPU_UNBOUND;

	 
	idx = !qid ? 0 : (qid - 1) % num_active_cpus();

	 
	for (cpu = 0, cnt = 0; ; ) {
		if (cpu_active(cpu)) {
			if (cnt == idx)
				break;
			cnt++;
		}
		cpu = (cpu + 1) % num_possible_cpus();
	}

	return cpu;
}