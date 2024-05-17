int restore_altstack(const stack_t __user *uss)
{
	int err = do_sigaltstack(uss, NULL, current_user_stack_pointer());
	 
	return err == -EFAULT ? err : 0;
}
