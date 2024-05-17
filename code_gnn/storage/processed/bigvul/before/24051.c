static int proc_APList_open( struct inode *inode, struct file *file ) {
	struct proc_data *data;
	struct proc_dir_entry *dp = PDE(inode);
	struct net_device *dev = dp->data;
	struct airo_info *ai = dev->ml_priv;
	int i;
	char *ptr;
	APListRid APList_rid;

	if ((file->private_data = kzalloc(sizeof(struct proc_data ), GFP_KERNEL)) == NULL)
		return -ENOMEM;
	data = file->private_data;
	if ((data->rbuffer = kmalloc( 104, GFP_KERNEL )) == NULL) {
		kfree (file->private_data);
		return -ENOMEM;
	}
	data->writelen = 0;
	data->maxwritelen = 4*6*3;
	if ((data->wbuffer = kzalloc( data->maxwritelen, GFP_KERNEL )) == NULL) {
		kfree (data->rbuffer);
		kfree (file->private_data);
		return -ENOMEM;
	}
	data->on_close = proc_APList_on_close;

	readAPListRid(ai, &APList_rid);
	ptr = data->rbuffer;
	for( i = 0; i < 4; i++ ) {
		if ( !*(int*)APList_rid.ap[i] &&
		     !*(int*)&APList_rid.ap[i][2]) break;
		ptr += sprintf(ptr, "%pM\n", APList_rid.ap[i]);
	}
	if (i==0) ptr += sprintf(ptr, "Not using specific APs\n");

	*ptr = '\0';
	data->readlen = strlen( data->rbuffer );
	return 0;
}