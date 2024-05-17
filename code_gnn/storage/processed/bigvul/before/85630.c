static void hns_xgmac_enable(void *mac_drv, enum mac_commom_mode mode)
{
	struct mac_driver *drv = (struct mac_driver *)mac_drv;

	hns_xgmac_lf_rf_insert(drv, HNS_XGMAC_NO_LF_RF_INSERT);

	 
	if (mode == MAC_COMM_MODE_TX) {
		hns_xgmac_tx_enable(drv, 1);
	} else if (mode == MAC_COMM_MODE_RX) {
		hns_xgmac_rx_enable(drv, 1);
	} else if (mode == MAC_COMM_MODE_RX_AND_TX) {
		hns_xgmac_tx_enable(drv, 1);
		hns_xgmac_rx_enable(drv, 1);
	} else {
		dev_err(drv->dev, "error mac mode:%d\n", mode);
	}
}