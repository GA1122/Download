SYSCALL_DEFINE1(osf_utsname, char __user *, name)
{
	int error;

	down_read(&uts_sem);
	error = -EFAULT;
	if (copy_to_user(name + 0, utsname()->sysname, 32))
		goto out;
	if (copy_to_user(name + 32, utsname()->nodename, 32))
		goto out;
	if (copy_to_user(name + 64, utsname()->release, 32))
		goto out;
	if (copy_to_user(name + 96, utsname()->version, 32))
		goto out;
	if (copy_to_user(name + 128, utsname()->machine, 32))
		goto out;

	error = 0;
 out:
	up_read(&uts_sem);	
	return error;
}
