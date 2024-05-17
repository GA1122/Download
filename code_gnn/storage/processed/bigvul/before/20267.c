int cap_file_mmap(struct file *file, unsigned long reqprot,
		  unsigned long prot, unsigned long flags,
		  unsigned long addr, unsigned long addr_only)
{
	int ret = 0;

	if (addr < dac_mmap_min_addr) {
		ret = cap_capable(current_cred(), &init_user_ns, CAP_SYS_RAWIO,
				  SECURITY_CAP_AUDIT);
		 
		if (ret == 0)
			current->flags |= PF_SUPERPRIV;
	}
	return ret;
}
