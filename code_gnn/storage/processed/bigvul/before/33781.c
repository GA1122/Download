long vhost_dev_check_owner(struct vhost_dev *dev)
{
	 
	return dev->mm == current->mm ? 0 : -EPERM;
}
