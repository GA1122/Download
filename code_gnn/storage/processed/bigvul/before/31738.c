int compat_restore_altstack(const compat_stack_t __user *uss)
{
	int err = compat_sys_sigaltstack(uss, NULL);
	 
	return err == -EFAULT ? err : 0;
}
