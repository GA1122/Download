static int atusb_probe(struct usb_interface *interface,
		       const struct usb_device_id *id)
{
	struct usb_device *usb_dev = interface_to_usbdev(interface);
	struct ieee802154_hw *hw;
	struct atusb *atusb = NULL;
	int ret = -ENOMEM;

	hw = ieee802154_alloc_hw(sizeof(struct atusb), &atusb_ops);
	if (!hw)
		return -ENOMEM;

	atusb = hw->priv;
	atusb->hw = hw;
	atusb->usb_dev = usb_get_dev(usb_dev);
	usb_set_intfdata(interface, atusb);

	atusb->shutdown = 0;
	atusb->err = 0;
	INIT_DELAYED_WORK(&atusb->work, atusb_work_urbs);
	init_usb_anchor(&atusb->idle_urbs);
	init_usb_anchor(&atusb->rx_urbs);

	if (atusb_alloc_urbs(atusb, ATUSB_NUM_RX_URBS))
		goto fail;

	atusb->tx_dr.bRequestType = ATUSB_REQ_TO_DEV;
	atusb->tx_dr.bRequest = ATUSB_TX;
	atusb->tx_dr.wValue = cpu_to_le16(0);

	atusb->tx_urb = usb_alloc_urb(0, GFP_ATOMIC);
	if (!atusb->tx_urb)
		goto fail;

	hw->parent = &usb_dev->dev;
	hw->flags = IEEE802154_HW_TX_OMIT_CKSUM | IEEE802154_HW_AFILT |
		    IEEE802154_HW_PROMISCUOUS | IEEE802154_HW_CSMA_PARAMS |
		    IEEE802154_HW_FRAME_RETRIES;

	hw->phy->flags = WPAN_PHY_FLAG_TXPOWER | WPAN_PHY_FLAG_CCA_ED_LEVEL |
			 WPAN_PHY_FLAG_CCA_MODE;

	hw->phy->supported.cca_modes = BIT(NL802154_CCA_ENERGY) |
		BIT(NL802154_CCA_CARRIER) | BIT(NL802154_CCA_ENERGY_CARRIER);
	hw->phy->supported.cca_opts = BIT(NL802154_CCA_OPT_ENERGY_CARRIER_AND) |
		BIT(NL802154_CCA_OPT_ENERGY_CARRIER_OR);

	hw->phy->supported.cca_ed_levels = atusb_ed_levels;
	hw->phy->supported.cca_ed_levels_size = ARRAY_SIZE(atusb_ed_levels);

	hw->phy->cca.mode = NL802154_CCA_ENERGY;

	hw->phy->current_page = 0;
	hw->phy->current_channel = 11;	 
	hw->phy->supported.channels[0] = 0x7FFF800;
	hw->phy->supported.tx_powers = atusb_powers;
	hw->phy->supported.tx_powers_size = ARRAY_SIZE(atusb_powers);
	hw->phy->transmit_power = hw->phy->supported.tx_powers[0];
	hw->phy->cca_ed_level = hw->phy->supported.cca_ed_levels[7];

	atusb_command(atusb, ATUSB_RF_RESET, 0);
	atusb_get_and_show_chip(atusb);
	atusb_get_and_show_revision(atusb);
	atusb_get_and_show_build(atusb);
	atusb_set_extended_addr(atusb);

	ret = atusb_get_and_clear_error(atusb);
	if (ret) {
		dev_err(&atusb->usb_dev->dev,
			"%s: initialization failed, error = %d\n",
			__func__, ret);
		goto fail;
	}

	ret = ieee802154_register_hw(hw);
	if (ret)
		goto fail;

	 
	atusb_write_reg(atusb, RG_TRX_STATE, STATE_FORCE_TRX_OFF);
	msleep(1);	 

#if 0
	 

	atusb_write_subreg(atusb, SR_RX_SAFE_MODE, 1);
#endif
	atusb_write_reg(atusb, RG_IRQ_MASK, 0xff);

	ret = atusb_get_and_clear_error(atusb);
	if (!ret)
		return 0;

	dev_err(&atusb->usb_dev->dev,
		"%s: setup failed, error = %d\n",
		__func__, ret);

	ieee802154_unregister_hw(hw);
fail:
	atusb_free_urbs(atusb);
	usb_kill_urb(atusb->tx_urb);
	usb_free_urb(atusb->tx_urb);
	usb_put_dev(usb_dev);
	ieee802154_free_hw(hw);
	return ret;
}
