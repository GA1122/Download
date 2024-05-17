static int hns_nic_init_affinity_mask(int q_num, int ring_idx,
				      struct hnae_ring *ring, cpumask_t *mask)
{
	int cpu;

	 
	if (q_num == num_possible_cpus()) {
		if (is_tx_ring(ring))
			cpu = ring_idx;
		else
			cpu = ring_idx - q_num;
	} else {
		if (is_tx_ring(ring))
			cpu = ring_idx * 2;
		else
			cpu = (ring_idx - q_num) * 2 + 1;
	}

	cpumask_clear(mask);
	cpumask_set_cpu(cpu, mask);

	return cpu;
}
