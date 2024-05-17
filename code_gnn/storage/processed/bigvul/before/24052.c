static int proc_BSSList_open( struct inode *inode, struct file *file ) {
	struct proc_data *data;
	struct proc_dir_entry *dp = PDE(inode);
	struct net_device *dev = dp->data;
	struct airo_info *ai = dev->ml_priv;
	char *ptr;
	BSSListRid BSSList_rid;
	int rc;
	 
	int doLoseSync = -1;

	if ((file->private_data = kzalloc(sizeof(struct proc_data ), GFP_KERNEL)) == NULL)
		return -ENOMEM;
	data = file->private_data;
	if ((data->rbuffer = kmalloc( 1024, GFP_KERNEL )) == NULL) {
		kfree (file->private_data);
		return -ENOMEM;
	}
	data->writelen = 0;
	data->maxwritelen = 0;
	data->wbuffer = NULL;
	data->on_close = NULL;

	if (file->f_mode & FMODE_WRITE) {
		if (!(file->f_mode & FMODE_READ)) {
			Cmd cmd;
			Resp rsp;

			if (ai->flags & FLAG_RADIO_MASK) return -ENETDOWN;
			memset(&cmd, 0, sizeof(cmd));
			cmd.cmd=CMD_LISTBSS;
			if (down_interruptible(&ai->sem))
				return -ERESTARTSYS;
			issuecommand(ai, &cmd, &rsp);
			up(&ai->sem);
			data->readlen = 0;
			return 0;
		}
		doLoseSync = 1;
	}
	ptr = data->rbuffer;
	 
	rc = readBSSListRid(ai, doLoseSync, &BSSList_rid);
	while(rc == 0 && BSSList_rid.index != cpu_to_le16(0xffff)) {
		ptr += sprintf(ptr, "%pM %*s rssi = %d",
			       BSSList_rid.bssid,
				(int)BSSList_rid.ssidLen,
				BSSList_rid.ssid,
				le16_to_cpu(BSSList_rid.dBm));
		ptr += sprintf(ptr, " channel = %d %s %s %s %s\n",
				le16_to_cpu(BSSList_rid.dsChannel),
				BSSList_rid.cap & CAP_ESS ? "ESS" : "",
				BSSList_rid.cap & CAP_IBSS ? "adhoc" : "",
				BSSList_rid.cap & CAP_PRIVACY ? "wep" : "",
				BSSList_rid.cap & CAP_SHORTHDR ? "shorthdr" : "");
		rc = readBSSListRid(ai, 0, &BSSList_rid);
	}
	*ptr = '\0';
	data->readlen = strlen( data->rbuffer );
	return 0;
}