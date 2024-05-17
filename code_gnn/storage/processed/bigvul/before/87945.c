static void fm10k_set_num_queues(struct fm10k_intfc *interface)
{
	 
	if (fm10k_set_qos_queues(interface))
		return;

	 
	fm10k_set_rss_queues(interface);
}
