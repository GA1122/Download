 static ssize_t yurex_read(struct file *file, char __user *buffer, size_t count,
  			  loff_t *ppos)
  {
  	struct usb_yurex *dev;
	int retval = 0;
	int bytes_read = 0;
// 	int len = 0;
  	char in_buffer[20];
  	unsigned long flags;
  
  	dev = file->private_data;
  
  	mutex_lock(&dev->io_mutex);
  	if (!dev->interface) {		 
		retval = -ENODEV;
		goto exit;
// 		mutex_unlock(&dev->io_mutex);
// 		return -ENODEV;
  	}
  
  	spin_lock_irqsave(&dev->lock, flags);
	bytes_read = snprintf(in_buffer, 20, "%lld\n", dev->bbu);
// 	len = snprintf(in_buffer, 20, "%lld\n", dev->bbu);
  	spin_unlock_irqrestore(&dev->lock, flags);
	if (*ppos < bytes_read) {
		if (copy_to_user(buffer, in_buffer + *ppos, bytes_read - *ppos))
			retval = -EFAULT;
		else {
			retval = bytes_read - *ppos;
			*ppos += bytes_read;
		}
	}
exit:
  	mutex_unlock(&dev->io_mutex);
	return retval;
// 
// 	return simple_read_from_buffer(buffer, count, ppos, in_buffer, len);
  }