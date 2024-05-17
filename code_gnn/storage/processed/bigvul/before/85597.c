void hns_rcb_get_common_regs(struct rcb_common_cb *rcb_com, void *data)
{
	u32 *regs = data;
	bool is_ver1 = AE_IS_VER1(rcb_com->dsaf_dev->dsaf_ver);
	bool is_dbg = HNS_DSAF_IS_DEBUG(rcb_com->dsaf_dev);
	u32 reg_tmp;
	u32 reg_num_tmp;
	u32 i = 0;

	 
	regs[0] = dsaf_read_dev(rcb_com, RCB_COM_CFG_ENDIAN_REG);
	regs[1] = dsaf_read_dev(rcb_com, RCB_COM_CFG_SYS_FSH_REG);
	regs[2] = dsaf_read_dev(rcb_com, RCB_COM_CFG_INIT_FLAG_REG);

	regs[3] = dsaf_read_dev(rcb_com, RCB_COM_CFG_PKT_REG);
	regs[4] = dsaf_read_dev(rcb_com, RCB_COM_CFG_RINVLD_REG);
	regs[5] = dsaf_read_dev(rcb_com, RCB_COM_CFG_FNA_REG);
	regs[6] = dsaf_read_dev(rcb_com, RCB_COM_CFG_FA_REG);
	regs[7] = dsaf_read_dev(rcb_com, RCB_COM_CFG_PKT_TC_BP_REG);
	regs[8] = dsaf_read_dev(rcb_com, RCB_COM_CFG_PPE_TNL_CLKEN_REG);

	regs[9] = dsaf_read_dev(rcb_com, RCB_COM_INTMSK_TX_PKT_REG);
	regs[10] = dsaf_read_dev(rcb_com, RCB_COM_RINT_TX_PKT_REG);
	regs[11] = dsaf_read_dev(rcb_com, RCB_COM_INTMASK_ECC_ERR_REG);
	regs[12] = dsaf_read_dev(rcb_com, RCB_COM_INTSTS_ECC_ERR_REG);
	regs[13] = dsaf_read_dev(rcb_com, RCB_COM_EBD_SRAM_ERR_REG);
	regs[14] = dsaf_read_dev(rcb_com, RCB_COM_RXRING_ERR_REG);
	regs[15] = dsaf_read_dev(rcb_com, RCB_COM_TXRING_ERR_REG);
	regs[16] = dsaf_read_dev(rcb_com, RCB_COM_TX_FBD_ERR_REG);
	regs[17] = dsaf_read_dev(rcb_com, RCB_SRAM_ECC_CHK_EN_REG);
	regs[18] = dsaf_read_dev(rcb_com, RCB_SRAM_ECC_CHK0_REG);
	regs[19] = dsaf_read_dev(rcb_com, RCB_SRAM_ECC_CHK1_REG);
	regs[20] = dsaf_read_dev(rcb_com, RCB_SRAM_ECC_CHK2_REG);
	regs[21] = dsaf_read_dev(rcb_com, RCB_SRAM_ECC_CHK3_REG);
	regs[22] = dsaf_read_dev(rcb_com, RCB_SRAM_ECC_CHK4_REG);
	regs[23] = dsaf_read_dev(rcb_com, RCB_SRAM_ECC_CHK5_REG);
	regs[24] = dsaf_read_dev(rcb_com, RCB_ECC_ERR_ADDR0_REG);
	regs[25] = dsaf_read_dev(rcb_com, RCB_ECC_ERR_ADDR3_REG);
	regs[26] = dsaf_read_dev(rcb_com, RCB_ECC_ERR_ADDR4_REG);
	regs[27] = dsaf_read_dev(rcb_com, RCB_ECC_ERR_ADDR5_REG);

	regs[28] = dsaf_read_dev(rcb_com, RCB_COM_SF_CFG_INTMASK_RING);
	regs[29] = dsaf_read_dev(rcb_com, RCB_COM_SF_CFG_RING_STS);
	regs[30] = dsaf_read_dev(rcb_com, RCB_COM_SF_CFG_RING);
	regs[31] = dsaf_read_dev(rcb_com, RCB_COM_SF_CFG_INTMASK_BD);
	regs[32] = dsaf_read_dev(rcb_com, RCB_COM_SF_CFG_BD_RINT_STS);
	regs[33] = dsaf_read_dev(rcb_com, RCB_COM_RCB_RD_BD_BUSY);
	regs[34] = dsaf_read_dev(rcb_com, RCB_COM_RCB_FBD_CRT_EN);
	regs[35] = dsaf_read_dev(rcb_com, RCB_COM_AXI_WR_ERR_INTMASK);
	regs[36] = dsaf_read_dev(rcb_com, RCB_COM_AXI_ERR_STS);
	regs[37] = dsaf_read_dev(rcb_com, RCB_COM_CHK_TX_FBD_NUM_REG);

	 
	for (i = 0; i < 16; i++) {  
		regs[38 + i]
			= dsaf_read_dev(rcb_com, RCB_CFG_BD_NUM_REG + 4 * i);
		regs[54 + i]
			= dsaf_read_dev(rcb_com, RCB_CFG_PKTLINE_REG + 4 * i);
	}

	reg_tmp = is_ver1 ? RCB_CFG_OVERTIME_REG : RCB_PORT_CFG_OVERTIME_REG;
	reg_num_tmp = (is_ver1 || is_dbg) ? 1 : 6;
	for (i = 0; i < reg_num_tmp; i++)
		regs[70 + i] = dsaf_read_dev(rcb_com, reg_tmp);

	regs[76] = dsaf_read_dev(rcb_com, RCB_CFG_PKTLINE_INT_NUM_REG);
	regs[77] = dsaf_read_dev(rcb_com, RCB_CFG_OVERTIME_INT_NUM_REG);

	 
	for (i = 78; i < 80; i++)
		regs[i] = 0xcccccccc;
}