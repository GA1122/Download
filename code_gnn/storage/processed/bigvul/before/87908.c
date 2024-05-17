static int __fm10k_maybe_stop_tx(struct fm10k_ring *tx_ring, u16 size)
{
	netif_stop_subqueue(tx_ring->netdev, tx_ring->queue_index);

	 
	smp_mb();

	 
	if (likely(fm10k_desc_unused(tx_ring) < size))
		return -EBUSY;

	 
	netif_start_subqueue(tx_ring->netdev, tx_ring->queue_index);
	++tx_ring->tx_stats.restart_queue;
	return 0;
}
