SYSCALL_DEFINE1(old_adjtimex, struct timex32 __user *, txc_p)
{
        struct timex txc;
	int ret;

	 
	if (copy_from_user(&txc, txc_p, offsetof(struct timex32, time)) ||
	    copy_from_user(&txc.tick, &txc_p->tick, sizeof(struct timex32) - 
			   offsetof(struct timex32, time)))
	  return -EFAULT;

	ret = do_adjtimex(&txc);	
	if (ret < 0)
	  return ret;
	
	 
	if (copy_to_user(txc_p, &txc, offsetof(struct timex32, time)) ||
	    (copy_to_user(&txc_p->tick, &txc.tick, sizeof(struct timex32) - 
			  offsetof(struct timex32, tick))) ||
	    (put_tv32(&txc_p->time, &txc.time)))
	  return -EFAULT;

	return ret;
}