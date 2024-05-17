bool fm10k_check_tx_hang(struct fm10k_ring *tx_ring)
{
	u32 tx_done = fm10k_get_tx_completed(tx_ring);
	u32 tx_done_old = tx_ring->tx_stats.tx_done_old;
	u32 tx_pending = fm10k_get_tx_pending(tx_ring, true);

	clear_check_for_tx_hang(tx_ring);

	 
	if (!tx_pending || (tx_done_old != tx_done)) {
		 
		tx_ring->tx_stats.tx_done_old = tx_done;
		 
		clear_bit(__FM10K_HANG_CHECK_ARMED, tx_ring->state);

		return false;
	}

	 
	return test_and_set_bit(__FM10K_HANG_CHECK_ARMED, tx_ring->state);
}
