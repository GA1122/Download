static void hns_xgmac_set_tx_auto_pause_frames(void *mac_drv, u16 enable)
{
	struct mac_driver *drv = (struct mac_driver *)mac_drv;

	dsaf_set_dev_bit(drv, XGMAC_MAC_PAUSE_CTRL_REG,
			 XGMAC_PAUSE_CTL_TX_B, !!enable);

	 
	if (enable)
		dsaf_write_dev(drv, XGMAC_MAC_PAUSE_TIME_REG, enable);
}