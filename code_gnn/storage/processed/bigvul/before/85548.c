static void hns_gmac_init(void *mac_drv)
{
	u32 port;
	struct mac_driver *drv = (struct mac_driver *)mac_drv;
	struct dsaf_device *dsaf_dev
		= (struct dsaf_device *)dev_get_drvdata(drv->dev);

	port = drv->mac_id;

	dsaf_dev->misc_op->ge_srst(dsaf_dev, port, 0);
	mdelay(10);
	dsaf_dev->misc_op->ge_srst(dsaf_dev, port, 1);
	mdelay(10);
	hns_gmac_disable(mac_drv, MAC_COMM_MODE_RX_AND_TX);
	hns_gmac_tx_loop_pkt_dis(mac_drv);
	if (drv->mac_cb->mac_type == HNAE_PORT_DEBUG)
		hns_gmac_set_uc_match(mac_drv, 0);

	hns_gmac_config_pad_and_crc(mac_drv, 1);

	dsaf_set_dev_bit(drv, GMAC_MODE_CHANGE_EN_REG,
			 GMAC_MODE_CHANGE_EB_B, 1);

	 
	dsaf_set_dev_field(drv, GMAC_TX_WATER_LINE_REG, GMAC_TX_WATER_LINE_MASK,
			   GMAC_TX_WATER_LINE_SHIFT, 8);
}