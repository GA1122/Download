static int ims_pcu_init_application_mode(struct ims_pcu *pcu)
{
	static atomic_t device_no = ATOMIC_INIT(-1);

	const struct ims_pcu_device_info *info;
	int error;

	error = ims_pcu_get_device_info(pcu);
	if (error) {
		 
		return error;
	}

	error = ims_pcu_identify_type(pcu, &pcu->device_id);
	if (error) {
		dev_err(pcu->dev,
			"Failed to identify device, error: %d\n", error);
		 
		return 0;
	}

	if (pcu->device_id >= ARRAY_SIZE(ims_pcu_device_info) ||
	    !ims_pcu_device_info[pcu->device_id].keymap) {
		dev_err(pcu->dev, "Device ID %d is not valid\n", pcu->device_id);
		 
		return 0;
	}

	 
	pcu->device_no = atomic_inc_return(&device_no);

	 
	if (pcu->device_id != IMS_PCU_PCU_B_DEVICE_ID) {
		error = sysfs_create_group(&pcu->dev->kobj,
					   &ims_pcu_ofn_attr_group);
		if (error)
			return error;
	}

	error = ims_pcu_setup_backlight(pcu);
	if (error)
		return error;

	info = &ims_pcu_device_info[pcu->device_id];
	error = ims_pcu_setup_buttons(pcu, info->keymap, info->keymap_len);
	if (error)
		goto err_destroy_backlight;

	if (info->has_gamepad) {
		error = ims_pcu_setup_gamepad(pcu);
		if (error)
			goto err_destroy_buttons;
	}

	pcu->setup_complete = true;

	return 0;

err_destroy_buttons:
	ims_pcu_destroy_buttons(pcu);
err_destroy_backlight:
	ims_pcu_destroy_backlight(pcu);
	return error;
}
