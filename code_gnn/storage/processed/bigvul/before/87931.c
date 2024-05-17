int fm10k_init_queueing_scheme(struct fm10k_intfc *interface)
{
	int err;

	 
	fm10k_set_num_queues(interface);

	 
	err = fm10k_init_msix_capability(interface);
	if (err) {
		dev_err(&interface->pdev->dev,
			"Unable to initialize MSI-X capability\n");
		goto err_init_msix;
	}

	 
	err = fm10k_alloc_q_vectors(interface);
	if (err) {
		dev_err(&interface->pdev->dev,
			"Unable to allocate queue vectors\n");
		goto err_alloc_q_vectors;
	}

	 
	fm10k_assign_rings(interface);

	 
	fm10k_init_reta(interface);

	return 0;

err_alloc_q_vectors:
	fm10k_reset_msix_capability(interface);
err_init_msix:
	fm10k_reset_num_queues(interface);
	return err;
}
