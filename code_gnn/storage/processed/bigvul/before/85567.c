void hns_ppe_get_regs(struct hns_ppe_cb *ppe_cb, void *data)
{
	struct ppe_common_cb *ppe_common = ppe_cb->ppe_common_cb;
	u32 *regs = data;
	u32 i;
	u32 offset;

	 
	regs[0] = dsaf_read_dev(ppe_common, PPE_COM_CFG_QID_MODE_REG);
	regs[1] = dsaf_read_dev(ppe_common, PPE_COM_INTEN_REG);
	regs[2] = dsaf_read_dev(ppe_common, PPE_COM_RINT_REG);
	regs[3] = dsaf_read_dev(ppe_common, PPE_COM_INTSTS_REG);
	regs[4] = dsaf_read_dev(ppe_common, PPE_COM_COMMON_CNT_CLR_CE_REG);

	for (i = 0; i < DSAF_TOTAL_QUEUE_NUM; i++) {
		offset = PPE_COM_HIS_RX_PKT_QID_DROP_CNT_REG + 0x4 * i;
		regs[5 + i] = dsaf_read_dev(ppe_common, offset);
		offset = PPE_COM_HIS_RX_PKT_QID_OK_CNT_REG + 0x4 * i;
		regs[5 + i + DSAF_TOTAL_QUEUE_NUM]
				= dsaf_read_dev(ppe_common, offset);
		offset = PPE_COM_HIS_TX_PKT_QID_ERR_CNT_REG + 0x4 * i;
		regs[5 + i + DSAF_TOTAL_QUEUE_NUM * 2]
				= dsaf_read_dev(ppe_common, offset);
		offset = PPE_COM_HIS_TX_PKT_QID_OK_CNT_REG + 0x4 * i;
		regs[5 + i + DSAF_TOTAL_QUEUE_NUM * 3]
				= dsaf_read_dev(ppe_common, offset);
	}

	 
	for (i = 521; i < 524; i++)
		regs[i] = 0xeeeeeeee;

	 
	regs[525] = dsaf_read_dev(ppe_cb, PPE_CFG_TX_FIFO_THRSLD_REG);
	regs[526] = dsaf_read_dev(ppe_cb, PPE_CFG_RX_FIFO_THRSLD_REG);
	regs[527] = dsaf_read_dev(ppe_cb, PPE_CFG_RX_FIFO_PAUSE_THRSLD_REG);
	regs[528] = dsaf_read_dev(ppe_cb, PPE_CFG_RX_FIFO_SW_BP_THRSLD_REG);
	regs[529] = dsaf_read_dev(ppe_cb, PPE_CFG_PAUSE_IDLE_CNT_REG);
	regs[530] = dsaf_read_dev(ppe_cb, PPE_CFG_BUS_CTRL_REG);
	regs[531] = dsaf_read_dev(ppe_cb, PPE_CFG_TNL_TO_BE_RST_REG);
	regs[532] = dsaf_read_dev(ppe_cb, PPE_CURR_TNL_CAN_RST_REG);

	regs[533] = dsaf_read_dev(ppe_cb, PPE_CFG_XGE_MODE_REG);
	regs[534] = dsaf_read_dev(ppe_cb, PPE_CFG_MAX_FRAME_LEN_REG);
	regs[535] = dsaf_read_dev(ppe_cb, PPE_CFG_RX_PKT_MODE_REG);
	regs[536] = dsaf_read_dev(ppe_cb, PPE_CFG_RX_VLAN_TAG_REG);
	regs[537] = dsaf_read_dev(ppe_cb, PPE_CFG_TAG_GEN_REG);
	regs[538] = dsaf_read_dev(ppe_cb, PPE_CFG_PARSE_TAG_REG);
	regs[539] = dsaf_read_dev(ppe_cb, PPE_CFG_PRO_CHECK_EN_REG);

	regs[540] = dsaf_read_dev(ppe_cb, PPE_INTEN_REG);
	regs[541] = dsaf_read_dev(ppe_cb, PPE_RINT_REG);
	regs[542] = dsaf_read_dev(ppe_cb, PPE_INTSTS_REG);
	regs[543] = dsaf_read_dev(ppe_cb, PPE_CFG_RX_PKT_INT_REG);

	regs[544] = dsaf_read_dev(ppe_cb, PPE_CFG_HEAT_DECT_TIME0_REG);
	regs[545] = dsaf_read_dev(ppe_cb, PPE_CFG_HEAT_DECT_TIME1_REG);

	 
	regs[546] = dsaf_read_dev(ppe_cb, PPE_HIS_RX_SW_PKT_CNT_REG);
	regs[547] = dsaf_read_dev(ppe_cb, PPE_HIS_RX_WR_BD_OK_PKT_CNT_REG);
	regs[548] = dsaf_read_dev(ppe_cb, PPE_HIS_RX_PKT_NO_BUF_CNT_REG);
	regs[549] = dsaf_read_dev(ppe_cb, PPE_HIS_TX_BD_CNT_REG);
	regs[550] = dsaf_read_dev(ppe_cb, PPE_HIS_TX_PKT_CNT_REG);
	regs[551] = dsaf_read_dev(ppe_cb, PPE_HIS_TX_PKT_OK_CNT_REG);
	regs[552] = dsaf_read_dev(ppe_cb, PPE_HIS_TX_PKT_EPT_CNT_REG);
	regs[553] = dsaf_read_dev(ppe_cb, PPE_HIS_TX_PKT_CS_FAIL_CNT_REG);
	regs[554] = dsaf_read_dev(ppe_cb, PPE_HIS_RX_APP_BUF_FAIL_CNT_REG);
	regs[555] = dsaf_read_dev(ppe_cb, PPE_HIS_RX_APP_BUF_WAIT_CNT_REG);
	regs[556] = dsaf_read_dev(ppe_cb, PPE_HIS_RX_PKT_DROP_FUL_CNT_REG);
	regs[557] = dsaf_read_dev(ppe_cb, PPE_HIS_RX_PKT_DROP_PRT_CNT_REG);

	regs[558] = dsaf_read_dev(ppe_cb, PPE_TNL_0_5_CNT_CLR_CE_REG);
	regs[559] = dsaf_read_dev(ppe_cb, PPE_CFG_AXI_DBG_REG);
	regs[560] = dsaf_read_dev(ppe_cb, PPE_HIS_PRO_ERR_REG);
	regs[561] = dsaf_read_dev(ppe_cb, PPE_HIS_TNL_FIFO_ERR_REG);
	regs[562] = dsaf_read_dev(ppe_cb, PPE_CURR_CFF_DATA_NUM_REG);
	regs[563] = dsaf_read_dev(ppe_cb, PPE_CURR_RX_ST_REG);
	regs[564] = dsaf_read_dev(ppe_cb, PPE_CURR_TX_ST_REG);
	regs[565] = dsaf_read_dev(ppe_cb, PPE_CURR_RX_FIFO0_REG);
	regs[566] = dsaf_read_dev(ppe_cb, PPE_CURR_RX_FIFO1_REG);
	regs[567] = dsaf_read_dev(ppe_cb, PPE_CURR_TX_FIFO0_REG);
	regs[568] = dsaf_read_dev(ppe_cb, PPE_CURR_TX_FIFO1_REG);
	regs[569] = dsaf_read_dev(ppe_cb, PPE_ECO0_REG);
	regs[570] = dsaf_read_dev(ppe_cb, PPE_ECO1_REG);
	regs[571] = dsaf_read_dev(ppe_cb, PPE_ECO2_REG);

	 
	for (i = 572; i < 576; i++)
		regs[i] = 0xeeeeeeee;
}