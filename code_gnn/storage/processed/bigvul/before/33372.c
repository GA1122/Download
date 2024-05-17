static int write_i2c_mem(struct edgeport_serial *serial,
		int start_address, int length, __u8 address_type, __u8 *buffer)
{
	struct device *dev = &serial->serial->dev->dev;
	int status = 0;
	int write_length;
	__be16 be_start_address;

	 

	 
	write_length = EPROM_PAGE_SIZE -
				(start_address & (EPROM_PAGE_SIZE - 1));

	if (write_length > length)
		write_length = length;

	dev_dbg(dev, "%s - BytesInFirstPage Addr = %x, length = %d\n",
		__func__, start_address, write_length);
	usb_serial_debug_data(dev, __func__, write_length, buffer);

	 
	be_start_address = cpu_to_be16(start_address);
	status = ti_vsend_sync(serial->serial->dev,
				UMPC_MEMORY_WRITE, (__u16)address_type,
				(__force __u16)be_start_address,
				buffer,	write_length);
	if (status) {
		dev_dbg(dev, "%s - ERROR %d\n", __func__, status);
		return status;
	}

	length		-= write_length;
	start_address	+= write_length;
	buffer		+= write_length;

	 
	while (length) {
		if (length > EPROM_PAGE_SIZE)
			write_length = EPROM_PAGE_SIZE;
		else
			write_length = length;

		dev_dbg(dev, "%s - Page Write Addr = %x, length = %d\n",
			__func__, start_address, write_length);
		usb_serial_debug_data(dev, __func__, write_length, buffer);

		 
		be_start_address = cpu_to_be16(start_address);
		status = ti_vsend_sync(serial->serial->dev, UMPC_MEMORY_WRITE,
				(__u16)address_type,
				(__force __u16)be_start_address,
				buffer, write_length);
		if (status) {
			dev_err(dev, "%s - ERROR %d\n", __func__, status);
			return status;
		}

		length		-= write_length;
		start_address	+= write_length;
		buffer		+= write_length;
	}
	return status;
}
