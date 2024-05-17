static int aac_cfg_open(struct inode *inode, struct file *file)
{
	struct aac_dev *aac;
	unsigned minor_number = iminor(inode);
	int err = -ENODEV;

	mutex_lock(&aac_mutex);   
	list_for_each_entry(aac, &aac_devices, entry) {
		if (aac->id == minor_number) {
			file->private_data = aac;
			err = 0;
			break;
		}
	}
	mutex_unlock(&aac_mutex);

	return err;
}
