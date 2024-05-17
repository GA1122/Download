asmlinkage long sys_adjtimex(struct timex __user *txc_p)
{
	struct timex txc;		 
	int ret;

	 
	if(copy_from_user(&txc, txc_p, sizeof(struct timex)))
		return -EFAULT;
	ret = do_adjtimex(&txc);
	return copy_to_user(txc_p, &txc, sizeof(struct timex)) ? -EFAULT : ret;
}