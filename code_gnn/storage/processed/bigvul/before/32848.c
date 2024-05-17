static int wdm_release(struct inode *inode, struct file *file)
{
	struct wdm_device *desc = file->private_data;

	mutex_lock(&wdm_mutex);

	 
	mutex_lock(&desc->wlock);
	desc->count--;
	mutex_unlock(&desc->wlock);

	if (!desc->count) {
		if (!test_bit(WDM_DISCONNECTING, &desc->flags)) {
			dev_dbg(&desc->intf->dev, "wdm_release: cleanup");
			kill_urbs(desc);
			desc->manage_power(desc->intf, 0);
		} else {
			 
			pr_debug(KBUILD_MODNAME " %s: device gone - cleaning up\n", __func__);
			cleanup(desc);
		}
	}
	mutex_unlock(&wdm_mutex);
	return 0;
}
