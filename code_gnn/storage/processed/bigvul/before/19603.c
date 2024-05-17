bool creds_are_invalid(const struct cred *cred)
{
	if (cred->magic != CRED_MAGIC)
		return true;
#ifdef CONFIG_SECURITY_SELINUX
	 
	if (selinux_is_enabled() && cred->security) {
		if ((unsigned long) cred->security < PAGE_SIZE)
			return true;
		if ((*(u32 *)cred->security & 0xffffff00) ==
		    (POISON_FREE << 24 | POISON_FREE << 16 | POISON_FREE << 8))
			return true;
	}
#endif
	return false;
}
