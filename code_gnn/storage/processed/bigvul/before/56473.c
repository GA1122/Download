static inline int get_sigset_t(sigset_t *set,
			       const compat_sigset_t __user *uset)
{
	compat_sigset_t s32;

	if (copy_from_user(&s32, uset, sizeof(*uset)))
		return -EFAULT;

	 
	switch (_NSIG_WORDS) {
	case 4: set->sig[3] = s32.sig[6] | (((long)s32.sig[7]) << 32);
	case 3: set->sig[2] = s32.sig[4] | (((long)s32.sig[5]) << 32);
	case 2: set->sig[1] = s32.sig[2] | (((long)s32.sig[3]) << 32);
	case 1: set->sig[0] = s32.sig[0] | (((long)s32.sig[1]) << 32);
	}
	return 0;
}