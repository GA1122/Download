static int enable_net_traffic(struct net_device *dev, struct usb_device *usb)
{
	__u16 linkpart;
	__u8 data[4];
	pegasus_t *pegasus = netdev_priv(dev);
	int ret;

	read_mii_word(pegasus, pegasus->phy, MII_LPA, &linkpart);
	data[0] = 0xc8;  
	data[1] = 0;
	if (linkpart & (ADVERTISE_100FULL | ADVERTISE_10FULL))
		data[1] |= 0x20;	 
	if (linkpart & (ADVERTISE_100FULL | ADVERTISE_100HALF))
		data[1] |= 0x10;	 
	if (mii_mode)
		data[1] = 0;
	data[2] = loopback ? 0x09 : 0x01;

	memcpy(pegasus->eth_regs, data, sizeof(data));
	ret = set_registers(pegasus, EthCtrl0, 3, data);

	if (usb_dev_id[pegasus->dev_index].vendor == VENDOR_LINKSYS ||
	    usb_dev_id[pegasus->dev_index].vendor == VENDOR_LINKSYS2 ||
	    usb_dev_id[pegasus->dev_index].vendor == VENDOR_DLINK) {
		u16 auxmode;
		read_mii_word(pegasus, 0, 0x1b, &auxmode);
		auxmode |= 4;
		write_mii_word(pegasus, 0, 0x1b, &auxmode);
	}

	return ret;
}