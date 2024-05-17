SYSCALL_DEFINE4(osf_mount, unsigned long, typenr, const char __user *, path,
		int, flag, void __user *, data)
{
	int retval;
	char *name;

	name = getname(path);
	retval = PTR_ERR(name);
	if (IS_ERR(name))
		goto out;
	switch (typenr) {
	case 1:
		retval = osf_ufs_mount(name, data, flag);
		break;
	case 6:
		retval = osf_cdfs_mount(name, data, flag);
		break;
	case 9:
		retval = osf_procfs_mount(name, data, flag);
		break;
	default:
		retval = -EINVAL;
		printk("osf_mount(%ld, %x)\n", typenr, flag);
	}
	putname(name);
 out:
	return retval;
}
