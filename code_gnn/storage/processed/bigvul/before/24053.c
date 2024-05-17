static void proc_SSID_on_close(struct inode *inode, struct file *file)
{
	struct proc_data *data = file->private_data;
	struct proc_dir_entry *dp = PDE(inode);
	struct net_device *dev = dp->data;
	struct airo_info *ai = dev->ml_priv;
	SsidRid SSID_rid;
	int i;
	char *p = data->wbuffer;
	char *end = p + data->writelen;

	if (!data->writelen)
		return;

	*end = '\n';  

	memset(&SSID_rid, 0, sizeof(SSID_rid));

	for (i = 0; i < 3 && p < end; i++) {
		int j = 0;
		 
		while (*p != '\n' && j < 32)
			SSID_rid.ssids[i].ssid[j++] = *p++;
		if (j == 0)
			break;
		SSID_rid.ssids[i].len = cpu_to_le16(j);
		 
		while (*p++ != '\n')
			;
	}
	if (i)
		SSID_rid.len = cpu_to_le16(sizeof(SSID_rid));
	disable_MAC(ai, 1);
	writeSsidRid(ai, &SSID_rid, 1);
	enable_MAC(ai, 1);
}
