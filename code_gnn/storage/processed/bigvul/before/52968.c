static int hfi1_file_open(struct inode *inode, struct file *fp)
{
	 
	fp->private_data = kzalloc(sizeof(struct hfi1_filedata), GFP_KERNEL);
	if (fp->private_data)  
		((struct hfi1_filedata *)fp->private_data)->rec_cpu_num = -1;
	return fp->private_data ? 0 : -ENOMEM;
}
