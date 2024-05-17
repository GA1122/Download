static int download_fw(struct edgeport_serial *serial)
{
	struct device *dev = &serial->serial->interface->dev;
	int status = 0;
	struct usb_interface_descriptor *interface;
	const struct firmware *fw;
	const char *fw_name = "edgeport/down3.bin";
	struct edgeport_fw_hdr *fw_hdr;

	status = request_firmware(&fw, fw_name, dev);
	if (status) {
		dev_err(dev, "Failed to load image \"%s\" err %d\n",
				fw_name, status);
		return status;
	}

	if (check_fw_sanity(serial, fw)) {
		status = -EINVAL;
		goto out;
	}

	fw_hdr = (struct edgeport_fw_hdr *)fw->data;

	 
	serial->fw_version = (fw_hdr->major_version << 8) +
			fw_hdr->minor_version;

	 
	serial->product_info.hardware_type = HARDWARE_TYPE_TIUMP;

	 
	serial->TI_I2C_Type = DTK_ADDR_SPACE_I2C_TYPE_II;

	status = choose_config(serial->serial->dev);
	if (status)
		goto out;

	interface = &serial->serial->interface->cur_altsetting->desc;
	if (!interface) {
		dev_err(dev, "%s - no interface set, error!\n", __func__);
		status = -ENODEV;
		goto out;
	}

	 
	if (interface->bNumEndpoints > 1) {
		serial->product_info.TiMode = TI_MODE_DOWNLOAD;
		status = do_download_mode(serial, fw);
	} else {
		 
		serial->product_info.TiMode = TI_MODE_CONFIGURING;
		status = do_boot_mode(serial, fw);
	}

out:
	release_firmware(fw);
	return status;
}
