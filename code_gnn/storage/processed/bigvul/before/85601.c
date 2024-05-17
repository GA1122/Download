void hns_rcb_get_ring_regs(struct hnae_queue *queue, void *data)
{
	u32 *regs = data;
	struct ring_pair_cb *ring_pair
		= container_of(queue, struct ring_pair_cb, q);
	u32 i = 0;

	 
	regs[0] = dsaf_read_dev(queue, RCB_RING_RX_RING_BASEADDR_L_REG);
	regs[1] = dsaf_read_dev(queue, RCB_RING_RX_RING_BASEADDR_H_REG);
	regs[2] = dsaf_read_dev(queue, RCB_RING_RX_RING_BD_NUM_REG);
	regs[3] = dsaf_read_dev(queue, RCB_RING_RX_RING_BD_LEN_REG);
	regs[4] = dsaf_read_dev(queue, RCB_RING_RX_RING_PKTLINE_REG);
	regs[5] = dsaf_read_dev(queue, RCB_RING_RX_RING_TAIL_REG);
	regs[6] = dsaf_read_dev(queue, RCB_RING_RX_RING_HEAD_REG);
	regs[7] = dsaf_read_dev(queue, RCB_RING_RX_RING_FBDNUM_REG);
	regs[8] = dsaf_read_dev(queue, RCB_RING_RX_RING_PKTNUM_RECORD_REG);

	regs[9] = dsaf_read_dev(queue, RCB_RING_TX_RING_BASEADDR_L_REG);
	regs[10] = dsaf_read_dev(queue, RCB_RING_TX_RING_BASEADDR_H_REG);
	regs[11] = dsaf_read_dev(queue, RCB_RING_TX_RING_BD_NUM_REG);
	regs[12] = dsaf_read_dev(queue, RCB_RING_TX_RING_BD_LEN_REG);
	regs[13] = dsaf_read_dev(queue, RCB_RING_TX_RING_PKTLINE_REG);
	regs[15] = dsaf_read_dev(queue, RCB_RING_TX_RING_TAIL_REG);
	regs[16] = dsaf_read_dev(queue, RCB_RING_TX_RING_HEAD_REG);
	regs[17] = dsaf_read_dev(queue, RCB_RING_TX_RING_FBDNUM_REG);
	regs[18] = dsaf_read_dev(queue, RCB_RING_TX_RING_OFFSET_REG);
	regs[19] = dsaf_read_dev(queue, RCB_RING_TX_RING_PKTNUM_RECORD_REG);

	regs[20] = dsaf_read_dev(queue, RCB_RING_PREFETCH_EN_REG);
	regs[21] = dsaf_read_dev(queue, RCB_RING_CFG_VF_NUM_REG);
	regs[22] = dsaf_read_dev(queue, RCB_RING_ASID_REG);
	regs[23] = dsaf_read_dev(queue, RCB_RING_RX_VM_REG);
	regs[24] = dsaf_read_dev(queue, RCB_RING_T0_BE_RST);
	regs[25] = dsaf_read_dev(queue, RCB_RING_COULD_BE_RST);
	regs[26] = dsaf_read_dev(queue, RCB_RING_WRR_WEIGHT_REG);

	regs[27] = dsaf_read_dev(queue, RCB_RING_INTMSK_RXWL_REG);
	regs[28] = dsaf_read_dev(queue, RCB_RING_INTSTS_RX_RING_REG);
	regs[29] = dsaf_read_dev(queue, RCB_RING_INTMSK_TXWL_REG);
	regs[30] = dsaf_read_dev(queue, RCB_RING_INTSTS_TX_RING_REG);
	regs[31] = dsaf_read_dev(queue, RCB_RING_INTMSK_RX_OVERTIME_REG);
	regs[32] = dsaf_read_dev(queue, RCB_RING_INTSTS_RX_OVERTIME_REG);
	regs[33] = dsaf_read_dev(queue, RCB_RING_INTMSK_TX_OVERTIME_REG);
	regs[34] = dsaf_read_dev(queue, RCB_RING_INTSTS_TX_OVERTIME_REG);

	 
	for (i = 35; i < 40; i++)
		regs[i] = 0xcccccc00 + ring_pair->index;
}