static int enable_net_traffic(rtl8150_t * dev)
{
	u8 cr, tcr, rcr, msr;

	if (!rtl8150_reset(dev)) {
		dev_warn(&dev->udev->dev, "device reset failed\n");
	}
	 
	rcr = 0x9e;
	tcr = 0xd8;
	cr = 0x0c;
	if (!(rcr & 0x80))
		set_bit(RTL8150_HW_CRC, &dev->flags);
	set_registers(dev, RCR, 1, &rcr);
	set_registers(dev, TCR, 1, &tcr);
	set_registers(dev, CR, 1, &cr);
	get_registers(dev, MSR, 1, &msr);

	return 0;
}