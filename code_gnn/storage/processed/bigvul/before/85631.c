static void hns_xgmac_exc_irq_en(struct mac_driver *drv, u32 en)
{
	u32 clr_vlue = 0xfffffffful;
	u32 msk_vlue = en ? 0xfffffffful : 0;  

	dsaf_write_dev(drv, XGMAC_INT_STATUS_REG, clr_vlue);
	dsaf_write_dev(drv, XGMAC_INT_ENABLE_REG, msk_vlue);
}