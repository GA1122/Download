void hns_rcb_get_stats(struct hnae_queue *queue, u64 *data)
{
	u64 *regs_buff = data;
	struct ring_pair_cb *ring =
		container_of(queue, struct ring_pair_cb, q);
	struct hns_ring_hw_stats *hw_stats = &ring->hw_stats;

	regs_buff[0] = hw_stats->tx_pkts;
	regs_buff[1] = hw_stats->ppe_tx_ok_pkts;
	regs_buff[2] = hw_stats->ppe_tx_drop_pkts;
	regs_buff[3] =
		dsaf_read_dev(queue, RCB_RING_TX_RING_FBDNUM_REG);

	regs_buff[4] = queue->tx_ring.stats.tx_pkts;
	regs_buff[5] = queue->tx_ring.stats.tx_bytes;
	regs_buff[6] = queue->tx_ring.stats.tx_err_cnt;
	regs_buff[7] = queue->tx_ring.stats.io_err_cnt;
	regs_buff[8] = queue->tx_ring.stats.sw_err_cnt;
	regs_buff[9] = queue->tx_ring.stats.seg_pkt_cnt;
	regs_buff[10] = queue->tx_ring.stats.restart_queue;
	regs_buff[11] = queue->tx_ring.stats.tx_busy;

	regs_buff[12] = hw_stats->rx_pkts;
	regs_buff[13] = hw_stats->ppe_rx_ok_pkts;
	regs_buff[14] = hw_stats->ppe_rx_drop_pkts;
	regs_buff[15] =
		dsaf_read_dev(queue, RCB_RING_RX_RING_FBDNUM_REG);

	regs_buff[16] = queue->rx_ring.stats.rx_pkts;
	regs_buff[17] = queue->rx_ring.stats.rx_bytes;
	regs_buff[18] = queue->rx_ring.stats.rx_err_cnt;
	regs_buff[19] = queue->rx_ring.stats.io_err_cnt;
	regs_buff[20] = queue->rx_ring.stats.sw_err_cnt;
	regs_buff[21] = queue->rx_ring.stats.seg_pkt_cnt;
	regs_buff[22] = queue->rx_ring.stats.reuse_pg_cnt;
	regs_buff[23] = queue->rx_ring.stats.err_pkt_len;
	regs_buff[24] = queue->rx_ring.stats.non_vld_descs;
	regs_buff[25] = queue->rx_ring.stats.err_bd_num;
	regs_buff[26] = queue->rx_ring.stats.l2_err;
	regs_buff[27] = queue->rx_ring.stats.l3l4_csum_err;
}