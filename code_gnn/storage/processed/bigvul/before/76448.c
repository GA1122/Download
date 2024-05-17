static inline struct uffd_msg userfault_msg(unsigned long address,
					    unsigned int flags,
					    unsigned long reason,
					    unsigned int features)
{
	struct uffd_msg msg;
	msg_init(&msg);
	msg.event = UFFD_EVENT_PAGEFAULT;
	msg.arg.pagefault.address = address;
	if (flags & FAULT_FLAG_WRITE)
		 
		msg.arg.pagefault.flags |= UFFD_PAGEFAULT_FLAG_WRITE;
	if (reason & VM_UFFD_WP)
		 
		msg.arg.pagefault.flags |= UFFD_PAGEFAULT_FLAG_WP;
	if (features & UFFD_FEATURE_THREAD_ID)
		msg.arg.pagefault.feat.ptid = task_pid_vnr(current);
	return msg;
}
