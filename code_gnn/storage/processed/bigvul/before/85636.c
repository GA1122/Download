static void hns_xgmac_get_regs(void *mac_drv, void *data)
{
	u32 i = 0;
	struct mac_driver *drv = (struct mac_driver *)mac_drv;
	u32 *regs = data;
	u64 qtmp;

	 
	regs[0] = dsaf_read_dev(drv, XGMAC_INT_STATUS_REG);
	regs[1] = dsaf_read_dev(drv, XGMAC_INT_ENABLE_REG);
	regs[2] = dsaf_read_dev(drv, XGMAC_INT_SET_REG);
	regs[3] = dsaf_read_dev(drv, XGMAC_IERR_U_INFO_REG);
	regs[4] = dsaf_read_dev(drv, XGMAC_OVF_INFO_REG);
	regs[5] = dsaf_read_dev(drv, XGMAC_OVF_CNT_REG);
	regs[6] = dsaf_read_dev(drv, XGMAC_PORT_MODE_REG);
	regs[7] = dsaf_read_dev(drv, XGMAC_CLK_ENABLE_REG);
	regs[8] = dsaf_read_dev(drv, XGMAC_RESET_REG);
	regs[9] = dsaf_read_dev(drv, XGMAC_LINK_CONTROL_REG);
	regs[10] = dsaf_read_dev(drv, XGMAC_LINK_STATUS_REG);

	regs[11] = dsaf_read_dev(drv, XGMAC_SPARE_REG);
	regs[12] = dsaf_read_dev(drv, XGMAC_SPARE_CNT_REG);
	regs[13] = dsaf_read_dev(drv, XGMAC_MAC_ENABLE_REG);
	regs[14] = dsaf_read_dev(drv, XGMAC_MAC_CONTROL_REG);
	regs[15] = dsaf_read_dev(drv, XGMAC_MAC_IPG_REG);
	regs[16] = dsaf_read_dev(drv, XGMAC_MAC_MSG_CRC_EN_REG);
	regs[17] = dsaf_read_dev(drv, XGMAC_MAC_MSG_IMG_REG);
	regs[18] = dsaf_read_dev(drv, XGMAC_MAC_MSG_FC_CFG_REG);
	regs[19] = dsaf_read_dev(drv, XGMAC_MAC_MSG_TC_CFG_REG);
	regs[20] = dsaf_read_dev(drv, XGMAC_MAC_PAD_SIZE_REG);
	regs[21] = dsaf_read_dev(drv, XGMAC_MAC_MIN_PKT_SIZE_REG);
	regs[22] = dsaf_read_dev(drv, XGMAC_MAC_MAX_PKT_SIZE_REG);
	regs[23] = dsaf_read_dev(drv, XGMAC_MAC_PAUSE_CTRL_REG);
	regs[24] = dsaf_read_dev(drv, XGMAC_MAC_PAUSE_TIME_REG);
	regs[25] = dsaf_read_dev(drv, XGMAC_MAC_PAUSE_GAP_REG);
	regs[26] = dsaf_read_dev(drv, XGMAC_MAC_PAUSE_LOCAL_MAC_H_REG);
	regs[27] = dsaf_read_dev(drv, XGMAC_MAC_PAUSE_LOCAL_MAC_L_REG);
	regs[28] = dsaf_read_dev(drv, XGMAC_MAC_PAUSE_PEER_MAC_H_REG);
	regs[29] = dsaf_read_dev(drv, XGMAC_MAC_PAUSE_PEER_MAC_L_REG);
	regs[30] = dsaf_read_dev(drv, XGMAC_MAC_PFC_PRI_EN_REG);
	regs[31] = dsaf_read_dev(drv, XGMAC_MAC_1588_CTRL_REG);
	regs[32] = dsaf_read_dev(drv, XGMAC_MAC_1588_TX_PORT_DLY_REG);
	regs[33] = dsaf_read_dev(drv, XGMAC_MAC_1588_RX_PORT_DLY_REG);
	regs[34] = dsaf_read_dev(drv, XGMAC_MAC_1588_ASYM_DLY_REG);
	regs[35] = dsaf_read_dev(drv, XGMAC_MAC_1588_ADJUST_CFG_REG);

	regs[36] = dsaf_read_dev(drv, XGMAC_MAC_Y1731_ETH_TYPE_REG);
	regs[37] = dsaf_read_dev(drv, XGMAC_MAC_MIB_CONTROL_REG);
	regs[38] = dsaf_read_dev(drv, XGMAC_MAC_WAN_RATE_ADJUST_REG);
	regs[39] = dsaf_read_dev(drv, XGMAC_MAC_TX_ERR_MARK_REG);
	regs[40] = dsaf_read_dev(drv, XGMAC_MAC_TX_LF_RF_CONTROL_REG);
	regs[41] = dsaf_read_dev(drv, XGMAC_MAC_RX_LF_RF_STATUS_REG);
	regs[42] = dsaf_read_dev(drv, XGMAC_MAC_TX_RUNT_PKT_CNT_REG);
	regs[43] = dsaf_read_dev(drv, XGMAC_MAC_RX_RUNT_PKT_CNT_REG);
	regs[44] = dsaf_read_dev(drv, XGMAC_MAC_RX_PREAM_ERR_PKT_CNT_REG);
	regs[45] = dsaf_read_dev(drv, XGMAC_MAC_TX_LF_RF_TERM_PKT_CNT_REG);
	regs[46] = dsaf_read_dev(drv, XGMAC_MAC_TX_SN_MISMATCH_PKT_CNT_REG);
	regs[47] = dsaf_read_dev(drv, XGMAC_MAC_RX_ERR_MSG_CNT_REG);
	regs[48] = dsaf_read_dev(drv, XGMAC_MAC_RX_ERR_EFD_CNT_REG);
	regs[49] = dsaf_read_dev(drv, XGMAC_MAC_ERR_INFO_REG);
	regs[50] = dsaf_read_dev(drv, XGMAC_MAC_DBG_INFO_REG);

	regs[51] = dsaf_read_dev(drv, XGMAC_PCS_BASER_SYNC_THD_REG);
	regs[52] = dsaf_read_dev(drv, XGMAC_PCS_STATUS1_REG);
	regs[53] = dsaf_read_dev(drv, XGMAC_PCS_BASER_STATUS1_REG);
	regs[54] = dsaf_read_dev(drv, XGMAC_PCS_BASER_STATUS2_REG);
	regs[55] = dsaf_read_dev(drv, XGMAC_PCS_BASER_SEEDA_0_REG);
	regs[56] = dsaf_read_dev(drv, XGMAC_PCS_BASER_SEEDA_1_REG);
	regs[57] = dsaf_read_dev(drv, XGMAC_PCS_BASER_SEEDB_0_REG);
	regs[58] = dsaf_read_dev(drv, XGMAC_PCS_BASER_SEEDB_1_REG);
	regs[59] = dsaf_read_dev(drv, XGMAC_PCS_BASER_TEST_CONTROL_REG);
	regs[60] = dsaf_read_dev(drv, XGMAC_PCS_BASER_TEST_ERR_CNT_REG);
	regs[61] = dsaf_read_dev(drv, XGMAC_PCS_DBG_INFO_REG);
	regs[62] = dsaf_read_dev(drv, XGMAC_PCS_DBG_INFO1_REG);
	regs[63] = dsaf_read_dev(drv, XGMAC_PCS_DBG_INFO2_REG);
	regs[64] = dsaf_read_dev(drv, XGMAC_PCS_DBG_INFO3_REG);

	regs[65] = dsaf_read_dev(drv, XGMAC_PMA_ENABLE_REG);
	regs[66] = dsaf_read_dev(drv, XGMAC_PMA_CONTROL_REG);
	regs[67] = dsaf_read_dev(drv, XGMAC_PMA_SIGNAL_STATUS_REG);
	regs[68] = dsaf_read_dev(drv, XGMAC_PMA_DBG_INFO_REG);
	regs[69] = dsaf_read_dev(drv, XGMAC_PMA_FEC_ABILITY_REG);
	regs[70] = dsaf_read_dev(drv, XGMAC_PMA_FEC_CONTROL_REG);
	regs[71] = dsaf_read_dev(drv, XGMAC_PMA_FEC_CORR_BLOCK_CNT__REG);
	regs[72] = dsaf_read_dev(drv, XGMAC_PMA_FEC_UNCORR_BLOCK_CNT__REG);

	 
#define hns_xgmac_cpy_q(p, q) \
	do {\
		*(p) = (u32)(q);\
		*((p) + 1) = (u32)((q) >> 32);\
	} while (0)

	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PKTS_FRAGMENT);
	hns_xgmac_cpy_q(&regs[73], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PKTS_UNDERSIZE);
	hns_xgmac_cpy_q(&regs[75], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PKTS_UNDERMIN);
	hns_xgmac_cpy_q(&regs[77], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PKTS_64OCTETS);
	hns_xgmac_cpy_q(&regs[79], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PKTS_65TO127OCTETS);
	hns_xgmac_cpy_q(&regs[81], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PKTS_128TO255OCTETS);
	hns_xgmac_cpy_q(&regs[83], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PKTS_256TO511OCTETS);
	hns_xgmac_cpy_q(&regs[85], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PKTS_512TO1023OCTETS);
	hns_xgmac_cpy_q(&regs[87], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PKTS_1024TO1518OCTETS);
	hns_xgmac_cpy_q(&regs[89], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PKTS_1519TOMAXOCTETS);
	hns_xgmac_cpy_q(&regs[91], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PKTS_1519TOMAXOCTETSOK);
	hns_xgmac_cpy_q(&regs[93], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PKTS_OVERSIZE);
	hns_xgmac_cpy_q(&regs[95], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PKTS_JABBER);
	hns_xgmac_cpy_q(&regs[97], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_GOODPKTS);
	hns_xgmac_cpy_q(&regs[99], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_GOODOCTETS);
	hns_xgmac_cpy_q(&regs[101], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_TOTAL_PKTS);
	hns_xgmac_cpy_q(&regs[103], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_TOTALOCTETS);
	hns_xgmac_cpy_q(&regs[105], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_UNICASTPKTS);
	hns_xgmac_cpy_q(&regs[107], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_MULTICASTPKTS);
	hns_xgmac_cpy_q(&regs[109], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_BROADCASTPKTS);
	hns_xgmac_cpy_q(&regs[111], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PRI0PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[113], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PRI1PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[115], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PRI2PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[117], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PRI3PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[119], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PRI4PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[121], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PRI5PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[123], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PRI6PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[125], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_PRI7PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[127], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_MACCTRLPKTS);
	hns_xgmac_cpy_q(&regs[129], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_1731PKTS);
	hns_xgmac_cpy_q(&regs[131], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_1588PKTS);
	hns_xgmac_cpy_q(&regs[133], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_FROMAPPGOODPKTS);
	hns_xgmac_cpy_q(&regs[135], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_FROMAPPBADPKTS);
	hns_xgmac_cpy_q(&regs[137], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_ERRALLPKTS);
	hns_xgmac_cpy_q(&regs[139], qtmp);

	 
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PKTS_FRAGMENT);
	hns_xgmac_cpy_q(&regs[141], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PKTSUNDERSIZE);
	hns_xgmac_cpy_q(&regs[143], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PKTS_UNDERMIN);
	hns_xgmac_cpy_q(&regs[145], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PKTS_64OCTETS);
	hns_xgmac_cpy_q(&regs[147], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PKTS_65TO127OCTETS);
	hns_xgmac_cpy_q(&regs[149], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PKTS_128TO255OCTETS);
	hns_xgmac_cpy_q(&regs[151], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PKTS_256TO511OCTETS);
	hns_xgmac_cpy_q(&regs[153], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PKTS_512TO1023OCTETS);
	hns_xgmac_cpy_q(&regs[155], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PKTS_1024TO1518OCTETS);
	hns_xgmac_cpy_q(&regs[157], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PKTS_1519TOMAXOCTETS);
	hns_xgmac_cpy_q(&regs[159], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PKTS_1519TOMAXOCTETSOK);
	hns_xgmac_cpy_q(&regs[161], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PKTS_OVERSIZE);
	hns_xgmac_cpy_q(&regs[163], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PKTS_JABBER);
	hns_xgmac_cpy_q(&regs[165], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_GOODPKTS);
	hns_xgmac_cpy_q(&regs[167], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_GOODOCTETS);
	hns_xgmac_cpy_q(&regs[169], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_TOTAL_PKTS);
	hns_xgmac_cpy_q(&regs[171], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_TOTALOCTETS);
	hns_xgmac_cpy_q(&regs[173], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_UNICASTPKTS);
	hns_xgmac_cpy_q(&regs[175], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_MULTICASTPKTS);
	hns_xgmac_cpy_q(&regs[177], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_BROADCASTPKTS);
	hns_xgmac_cpy_q(&regs[179], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PRI0PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[181], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PRI1PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[183], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PRI2PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[185], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PRI3PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[187], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PRI4PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[189], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PRI5PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[191], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PRI6PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[193], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_PRI7PAUSEPKTS);
	hns_xgmac_cpy_q(&regs[195], qtmp);

	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_MACCTRLPKTS);
	hns_xgmac_cpy_q(&regs[197], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_SENDAPPGOODPKTS);
	hns_xgmac_cpy_q(&regs[199], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_TX_SENDAPPBADPKTS);
	hns_xgmac_cpy_q(&regs[201], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_1731PKTS);
	hns_xgmac_cpy_q(&regs[203], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_SYMBOLERRPKTS);
	hns_xgmac_cpy_q(&regs[205], qtmp);
	qtmp = hns_mac_reg_read64(drv, XGMAC_RX_FCSERRPKTS);
	hns_xgmac_cpy_q(&regs[207], qtmp);

	 
	for (i = 208; i < 214; i++)
		regs[i] = 0xaaaaaaaa;
}