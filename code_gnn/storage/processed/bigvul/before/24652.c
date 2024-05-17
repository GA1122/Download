static int mmtimer_ioctl(struct inode *inode, struct file *file,
			 unsigned int cmd, unsigned long arg)
{
	int ret = 0;

	switch (cmd) {
	case MMTIMER_GETOFFSET:	 
		 
		if(PAGE_SIZE <= (1 << 16))
			ret = (((long)RTC_COUNTER_ADDR) & (PAGE_SIZE-1)) / 8;
		else
			ret = -ENOSYS;
		break;

	case MMTIMER_GETRES:  
		if(copy_to_user((unsigned long __user *)arg,
				&mmtimer_femtoperiod, sizeof(unsigned long)))
			return -EFAULT;
		break;

	case MMTIMER_GETFREQ:  
		if(copy_to_user((unsigned long __user *)arg,
				&sn_rtc_cycles_per_second,
				sizeof(unsigned long)))
			return -EFAULT;
		ret = 0;
		break;

	case MMTIMER_GETBITS:  
		ret = RTC_BITS;
		break;

	case MMTIMER_MMAPAVAIL:  
		ret = (PAGE_SIZE <= (1 << 16)) ? 1 : 0;
		break;

	case MMTIMER_GETCOUNTER:
		if(copy_to_user((unsigned long __user *)arg,
				RTC_COUNTER_ADDR, sizeof(unsigned long)))
			return -EFAULT;
		break;
	default:
		ret = -ENOSYS;
		break;
	}

	return ret;
}
