static int proc_SSID_open(struct inode *inode, struct file *file)
{
	struct proc_data *data;
	struct proc_dir_entry *dp = PDE(inode);
	struct net_device *dev = dp->data;
	struct airo_info *ai = dev->ml_priv;
	int i;
	char *ptr;
	SsidRid SSID_rid;

	if ((file->private_data = kzalloc(sizeof(struct proc_data ), GFP_KERNEL)) == NULL)
		return -ENOMEM;
	data = file->private_data;
	if ((data->rbuffer = kmalloc( 104, GFP_KERNEL )) == NULL) {
		kfree (file->private_data);
		return -ENOMEM;
	}
	data->writelen = 0;
	data->maxwritelen = 33*3;
	 
	if ((data->wbuffer = kzalloc(33*3 + 1, GFP_KERNEL)) == NULL) {
		kfree (data->rbuffer);
		kfree (file->private_data);
		return -ENOMEM;
	}
	data->on_close = proc_SSID_on_close;

	readSsidRid(ai, &SSID_rid);
	ptr = data->rbuffer;
	for (i = 0; i < 3; i++) {
		int j;
		size_t len = le16_to_cpu(SSID_rid.ssids[i].len);
		if (!len)
			break;
		if (len > 32)
			len = 32;
		for (j = 0; j < len && SSID_rid.ssids[i].ssid[j]; j++)
			*ptr++ = SSID_rid.ssids[i].ssid[j];
		*ptr++ = '\n';
	}
	*ptr = '\0';
	data->readlen = strlen( data->rbuffer );
	return 0;
}