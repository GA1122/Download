static int cdrom_select_disc(struct cdrom_device_info *cdi, int slot)
{
	struct cdrom_changer_info *info;
	int curslot;
	int ret;

	cd_dbg(CD_CHANGER, "entering cdrom_select_disc()\n");
	if (!CDROM_CAN(CDC_SELECT_DISC))
		return -EDRIVE_CANT_DO_THIS;

	if (cdi->ops->check_events)
		cdi->ops->check_events(cdi, 0, slot);
	else
		cdi->ops->media_changed(cdi, slot);

	if (slot == CDSL_NONE) {
		 
		cdi->mc_flags = 0x3;
		return cdrom_load_unload(cdi, -1);
	}

	info = kmalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	if ((ret = cdrom_read_mech_status(cdi, info))) {
		kfree(info);
		return ret;
	}

	curslot = info->hdr.curslot;
	kfree(info);

	if (cdi->use_count > 1 || cdi->keeplocked) {
		if (slot == CDSL_CURRENT) {
	    		return curslot;
		} else {
			return -EBUSY;
		}
	}

	 
	if (slot == CDSL_CURRENT)
		slot = curslot;

	 
	cdi->mc_flags = 0x3;
	if ((ret = cdrom_load_unload(cdi, slot)))
		return ret;

	return slot;
}
