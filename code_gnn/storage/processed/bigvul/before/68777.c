static int atusb_set_extended_addr(struct atusb *atusb)
{
	struct usb_device *usb_dev = atusb->usb_dev;
	unsigned char buffer[IEEE802154_EXTENDED_ADDR_LEN];
	__le64 extended_addr;
	u64 addr;
	int ret;

	 
	if (atusb->fw_ver_maj == 0 && atusb->fw_ver_min < 3) {
		ieee802154_random_extended_addr(&atusb->hw->phy->perm_extended_addr);
		return 0;
	}

	 
	ret = atusb_control_msg(atusb, usb_rcvctrlpipe(usb_dev, 0),
				ATUSB_EUI64_READ, ATUSB_REQ_FROM_DEV, 0, 0,
				buffer, IEEE802154_EXTENDED_ADDR_LEN, 1000);
	if (ret < 0)
		dev_err(&usb_dev->dev, "failed to fetch extended address\n");

	memcpy(&extended_addr, buffer, IEEE802154_EXTENDED_ADDR_LEN);
	 
	if (!ieee802154_is_valid_extended_unicast_addr(extended_addr)) {
		dev_info(&usb_dev->dev, "no permanent extended address found, random address set\n");
		ieee802154_random_extended_addr(&atusb->hw->phy->perm_extended_addr);
	} else {
		atusb->hw->phy->perm_extended_addr = extended_addr;
		addr = swab64((__force u64)atusb->hw->phy->perm_extended_addr);
		dev_info(&usb_dev->dev, "Read permanent extended address %8phC from device\n",
			&addr);
	}

	return ret;
}