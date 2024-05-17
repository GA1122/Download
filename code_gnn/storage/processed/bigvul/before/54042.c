static ssize_t ims_pcu_update_firmware_store(struct device *dev,
					     struct device_attribute *dattr,
					     const char *buf, size_t count)
{
	struct usb_interface *intf = to_usb_interface(dev);
	struct ims_pcu *pcu = usb_get_intfdata(intf);
	const struct firmware *fw = NULL;
	int value;
	int error;

	error = kstrtoint(buf, 0, &value);
	if (error)
		return error;

	if (value != 1)
		return -EINVAL;

	error = mutex_lock_interruptible(&pcu->cmd_mutex);
	if (error)
		return error;

	error = request_ihex_firmware(&fw, IMS_PCU_FIRMWARE_NAME, pcu->dev);
	if (error) {
		dev_err(pcu->dev, "Failed to request firmware %s, error: %d\n",
			IMS_PCU_FIRMWARE_NAME, error);
		goto out;
	}

	 
	if (pcu->bootloader_mode)
		error = ims_pcu_handle_firmware_update(pcu, fw);
	else
		error = ims_pcu_switch_to_bootloader(pcu);

	release_firmware(fw);

out:
	mutex_unlock(&pcu->cmd_mutex);
	return error ?: count;
}
