static int fm10k_init_msix_capability(struct fm10k_intfc *interface)
{
	struct fm10k_hw *hw = &interface->hw;
	int v_budget, vector;

	 
	v_budget = max(interface->num_rx_queues, interface->num_tx_queues);
	v_budget = min_t(u16, v_budget, num_online_cpus());

	 
	v_budget += NON_Q_VECTORS(hw);

	 
	v_budget = min_t(int, v_budget, hw->mac.max_msix_vectors);

	 
	interface->msix_entries = kcalloc(v_budget, sizeof(struct msix_entry),
					  GFP_KERNEL);
	if (!interface->msix_entries)
		return -ENOMEM;

	 
	for (vector = 0; vector < v_budget; vector++)
		interface->msix_entries[vector].entry = vector;

	 
	v_budget = pci_enable_msix_range(interface->pdev,
					 interface->msix_entries,
					 MIN_MSIX_COUNT(hw),
					 v_budget);
	if (v_budget < 0) {
		kfree(interface->msix_entries);
		interface->msix_entries = NULL;
		return v_budget;
	}

	 
	interface->num_q_vectors = v_budget - NON_Q_VECTORS(hw);

	return 0;
}
