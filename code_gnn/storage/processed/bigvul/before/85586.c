static void hns_rcb_comm_exc_irq_en(
			struct rcb_common_cb *rcb_common, int en)
{
	u32 clr_vlue = 0xfffffffful;
	u32 msk_vlue = en ? 0 : 0xfffffffful;

	 
	dsaf_write_dev(rcb_common, RCB_COM_INTSTS_ECC_ERR_REG, clr_vlue);

	dsaf_write_dev(rcb_common, RCB_COM_SF_CFG_RING_STS, clr_vlue);

	dsaf_write_dev(rcb_common, RCB_COM_SF_CFG_BD_RINT_STS, clr_vlue);

	dsaf_write_dev(rcb_common, RCB_COM_RINT_TX_PKT_REG, clr_vlue);
	dsaf_write_dev(rcb_common, RCB_COM_AXI_ERR_STS, clr_vlue);

	 
	dsaf_write_dev(rcb_common, RCB_COM_INTMASK_ECC_ERR_REG, msk_vlue);

	dsaf_write_dev(rcb_common, RCB_COM_SF_CFG_INTMASK_RING, msk_vlue);

	 
	dsaf_write_dev(rcb_common, RCB_COM_SF_CFG_INTMASK_BD, msk_vlue | 2);

	dsaf_write_dev(rcb_common, RCB_COM_INTMSK_TX_PKT_REG, msk_vlue);
	dsaf_write_dev(rcb_common, RCB_COM_AXI_WR_ERR_INTMASK, msk_vlue);
}