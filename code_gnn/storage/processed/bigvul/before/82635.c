void __user *compat_alloc_user_space(unsigned long len)
{
	void __user *ptr;

	 
	if (unlikely(len > (((compat_uptr_t)~0) >> 1)))
		return NULL;

	ptr = arch_compat_alloc_user_space(len);

	if (unlikely(!access_ok(VERIFY_WRITE, ptr, len)))
		return NULL;

	return ptr;
}
