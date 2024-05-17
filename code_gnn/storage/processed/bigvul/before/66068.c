static int do_boot_mode(struct edgeport_serial *serial,
		const struct firmware *fw)
{
	struct device *dev = &serial->serial->interface->dev;
	int status = 0;
	struct edge_ti_manuf_descriptor *ti_manuf_desc;
	struct edgeport_fw_hdr *fw_hdr = (struct edgeport_fw_hdr *)fw->data;

	dev_dbg(dev, "%s - RUNNING IN BOOT MODE\n", __func__);

	 
	status = config_boot_dev(serial->serial->dev);
	if (status)
		return status;

	if (le16_to_cpu(serial->serial->dev->descriptor.idVendor)
							!= USB_VENDOR_ID_ION) {
		dev_dbg(dev, "%s - VID = 0x%x\n", __func__,
			le16_to_cpu(serial->serial->dev->descriptor.idVendor));
		serial->TI_I2C_Type = DTK_ADDR_SPACE_I2C_TYPE_II;
		goto stayinbootmode;
	}

	 
	if (i2c_type_bootmode(serial))
		goto stayinbootmode;

	 
	if (!check_i2c_image(serial)) {
		struct ti_i2c_image_header *header;
		int i;
		__u8 cs = 0;
		__u8 *buffer;
		int buffer_size;

		 
		ti_manuf_desc = kmalloc(sizeof(*ti_manuf_desc), GFP_KERNEL);
		if (!ti_manuf_desc)
			return -ENOMEM;

		status = get_manuf_info(serial, (__u8 *)ti_manuf_desc);
		if (status) {
			kfree(ti_manuf_desc);
			goto stayinbootmode;
		}

		 
		if (!ignore_cpu_rev && ti_cpu_rev(ti_manuf_desc) < 2) {
			dev_dbg(dev, "%s - Wrong CPU Rev %d (Must be 2)\n",
				__func__, ti_cpu_rev(ti_manuf_desc));
			kfree(ti_manuf_desc);
			goto stayinbootmode;
		}

		kfree(ti_manuf_desc);

		 

		 
		buffer_size = (((1024 * 16) - 512) +
					sizeof(struct ti_i2c_image_header));
		buffer = kmalloc(buffer_size, GFP_KERNEL);
		if (!buffer)
			return -ENOMEM;

		 
		memset(buffer, 0xff, buffer_size);
		memcpy(buffer, &fw->data[4], fw->size - 4);

		for (i = sizeof(struct ti_i2c_image_header);
				i < buffer_size; i++) {
			cs = (__u8)(cs + buffer[i]);
		}

		header = (struct ti_i2c_image_header *)buffer;

		 
		header->Length 	 = cpu_to_le16((__u16)(buffer_size -
					sizeof(struct ti_i2c_image_header)));
		header->CheckSum = cs;

		 
		dev_dbg(dev, "%s - Downloading operational code image version %d.%d (TI UMP)\n",
				__func__,
				fw_hdr->major_version, fw_hdr->minor_version);
		status = download_code(serial, buffer, buffer_size);

		kfree(buffer);

		if (status) {
			dev_dbg(dev, "%s - Error downloading operational code image\n", __func__);
			return status;
		}

		 
		serial->product_info.TiMode = TI_MODE_TRANSITIONING;

		dev_dbg(dev, "%s - Download successful -- Device rebooting...\n", __func__);

		return 1;
	}

stayinbootmode:
	 
	dev_dbg(dev, "%s - STAYING IN BOOT MODE\n", __func__);
	serial->product_info.TiMode = TI_MODE_BOOT;

	return 1;
}
