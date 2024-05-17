static int tg_shares_up(struct task_group *tg, void *data)
{
	unsigned long weight, rq_weight = 0, sum_weight = 0, shares = 0;
	unsigned long *usd_rq_weight;
	struct sched_domain *sd = data;
	unsigned long flags;
	int i;

	if (!tg->se[0])
		return 0;

	local_irq_save(flags);
	usd_rq_weight = per_cpu_ptr(update_shares_data, smp_processor_id());

	for_each_cpu(i, sched_domain_span(sd)) {
		weight = tg->cfs_rq[i]->load.weight;
		usd_rq_weight[i] = weight;

		rq_weight += weight;
		 
		if (!weight)
			weight = NICE_0_LOAD;

		sum_weight += weight;
		shares += tg->cfs_rq[i]->shares;
	}

	if (!rq_weight)
		rq_weight = sum_weight;

	if ((!shares && rq_weight) || shares > tg->shares)
		shares = tg->shares;

	if (!sd->parent || !(sd->parent->flags & SD_LOAD_BALANCE))
		shares = tg->shares;

	for_each_cpu(i, sched_domain_span(sd))
		update_group_shares_cpu(tg, i, shares, rq_weight, usd_rq_weight);

	local_irq_restore(flags);

	return 0;
}