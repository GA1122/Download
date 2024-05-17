SYSCALL_DEFINE0(sgetmask)
{
	 
	return current->blocked.sig[0];
}
