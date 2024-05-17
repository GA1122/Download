static int unlock_futex_pi(u32 __user *uaddr, u32 uval)
{
	u32 oldval;

	 
	oldval = cmpxchg_futex_value_locked(uaddr, uval, 0);

	if (oldval == -EFAULT)
		return oldval;
	if (oldval != uval)
		return -EAGAIN;

	return 0;
}
