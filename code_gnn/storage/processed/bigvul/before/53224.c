static int proc_drop_privileges(struct usb_dev_state *ps, void __user *arg)
{
	u32 data;

	if (copy_from_user(&data, arg, sizeof(data)))
		return -EFAULT;

	 
	ps->interface_allowed_mask &= data;
	ps->privileges_dropped = true;

	return 0;
}
