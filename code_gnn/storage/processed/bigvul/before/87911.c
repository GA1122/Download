static int fm10k_alloc_q_vector(struct fm10k_intfc *interface,
				unsigned int v_count, unsigned int v_idx,
				unsigned int txr_count, unsigned int txr_idx,
				unsigned int rxr_count, unsigned int rxr_idx)
{
	struct fm10k_q_vector *q_vector;
	struct fm10k_ring *ring;
	int ring_count;

	ring_count = txr_count + rxr_count;

	 
	q_vector = kzalloc(struct_size(q_vector, ring, ring_count), GFP_KERNEL);
	if (!q_vector)
		return -ENOMEM;

	 
	netif_napi_add(interface->netdev, &q_vector->napi,
		       fm10k_poll, NAPI_POLL_WEIGHT);

	 
	interface->q_vector[v_idx] = q_vector;
	q_vector->interface = interface;
	q_vector->v_idx = v_idx;

	 
	ring = q_vector->ring;

	 
	q_vector->tx.ring = ring;
	q_vector->tx.work_limit = FM10K_DEFAULT_TX_WORK;
	q_vector->tx.itr = interface->tx_itr;
	q_vector->tx.itr_scale = interface->hw.mac.itr_scale;
	q_vector->tx.count = txr_count;

	while (txr_count) {
		 
		ring->dev = &interface->pdev->dev;
		ring->netdev = interface->netdev;

		 
		ring->q_vector = q_vector;

		 
		ring->count = interface->tx_ring_count;
		ring->queue_index = txr_idx;

		 
		interface->tx_ring[txr_idx] = ring;

		 
		txr_count--;
		txr_idx += v_count;

		 
		ring++;
	}

	 
	q_vector->rx.ring = ring;
	q_vector->rx.itr = interface->rx_itr;
	q_vector->rx.itr_scale = interface->hw.mac.itr_scale;
	q_vector->rx.count = rxr_count;

	while (rxr_count) {
		 
		ring->dev = &interface->pdev->dev;
		ring->netdev = interface->netdev;
		rcu_assign_pointer(ring->l2_accel, interface->l2_accel);

		 
		ring->q_vector = q_vector;

		 
		ring->count = interface->rx_ring_count;
		ring->queue_index = rxr_idx;

		 
		interface->rx_ring[rxr_idx] = ring;

		 
		rxr_count--;
		rxr_idx += v_count;

		 
		ring++;
	}

	fm10k_dbg_q_vector_init(q_vector);

	return 0;
}
