void hns_ppe_update_stats(struct hns_ppe_cb *ppe_cb)
{
	struct hns_ppe_hw_stats *hw_stats = &ppe_cb->hw_stats;

	hw_stats->rx_pkts_from_sw
		+= dsaf_read_dev(ppe_cb, PPE_HIS_RX_SW_PKT_CNT_REG);
	hw_stats->rx_pkts
		+= dsaf_read_dev(ppe_cb, PPE_HIS_RX_WR_BD_OK_PKT_CNT_REG);
	hw_stats->rx_drop_no_bd
		+= dsaf_read_dev(ppe_cb, PPE_HIS_RX_PKT_NO_BUF_CNT_REG);
	hw_stats->rx_alloc_buf_fail
		+= dsaf_read_dev(ppe_cb, PPE_HIS_RX_APP_BUF_FAIL_CNT_REG);
	hw_stats->rx_alloc_buf_wait
		+= dsaf_read_dev(ppe_cb, PPE_HIS_RX_APP_BUF_WAIT_CNT_REG);
	hw_stats->rx_drop_no_buf
		+= dsaf_read_dev(ppe_cb, PPE_HIS_RX_PKT_DROP_FUL_CNT_REG);
	hw_stats->rx_err_fifo_full
		+= dsaf_read_dev(ppe_cb, PPE_HIS_RX_PKT_DROP_PRT_CNT_REG);

	hw_stats->tx_bd_form_rcb
		+= dsaf_read_dev(ppe_cb, PPE_HIS_TX_BD_CNT_REG);
	hw_stats->tx_pkts_from_rcb
		+= dsaf_read_dev(ppe_cb, PPE_HIS_TX_PKT_CNT_REG);
	hw_stats->tx_pkts
		+= dsaf_read_dev(ppe_cb, PPE_HIS_TX_PKT_OK_CNT_REG);
	hw_stats->tx_err_fifo_empty
		+= dsaf_read_dev(ppe_cb, PPE_HIS_TX_PKT_EPT_CNT_REG);
	hw_stats->tx_err_checksum
		+= dsaf_read_dev(ppe_cb, PPE_HIS_TX_PKT_CS_FAIL_CNT_REG);
}