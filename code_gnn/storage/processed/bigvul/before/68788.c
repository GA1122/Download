 static int atusb_write_subreg(struct atusb *atusb, uint8_t reg, uint8_t mask,
			      uint8_t shift, uint8_t value)
{
	struct usb_device *usb_dev = atusb->usb_dev;
	uint8_t orig, tmp;
	int ret = 0;

	dev_dbg(&usb_dev->dev, "atusb_write_subreg: 0x%02x <- 0x%02x\n",
		reg, value);

	orig = atusb_read_reg(atusb, reg);

	 
	tmp = orig & ~mask;
	tmp |= (value << shift) & mask;

	if (tmp != orig)
		ret = atusb_write_reg(atusb, reg, tmp);

	return ret;
}
