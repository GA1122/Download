static int fm10k_poll(struct napi_struct *napi, int budget)
{
	struct fm10k_q_vector *q_vector =
			       container_of(napi, struct fm10k_q_vector, napi);
	struct fm10k_ring *ring;
	int per_ring_budget, work_done = 0;
	bool clean_complete = true;

	fm10k_for_each_ring(ring, q_vector->tx) {
		if (!fm10k_clean_tx_irq(q_vector, ring, budget))
			clean_complete = false;
	}

	 
	if (budget <= 0)
		return budget;

	 
	if (q_vector->rx.count > 1)
		per_ring_budget = max(budget / q_vector->rx.count, 1);
	else
		per_ring_budget = budget;

	fm10k_for_each_ring(ring, q_vector->rx) {
		int work = fm10k_clean_rx_irq(q_vector, ring, per_ring_budget);

		work_done += work;
		if (work >= per_ring_budget)
			clean_complete = false;
	}

	 
	if (!clean_complete)
		return budget;

	 
	if (likely(napi_complete_done(napi, work_done)))
		fm10k_qv_enable(q_vector);

	return min(work_done, budget - 1);
}
