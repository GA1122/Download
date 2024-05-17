user_backtrace(struct frame_tail __user *tail,
	       struct perf_callchain_entry *entry)
{
	struct frame_tail buftail;
	unsigned long err;

	 
	if (!access_ok(VERIFY_READ, tail, sizeof(buftail)))
		return NULL;

	pagefault_disable();
	err = __copy_from_user_inatomic(&buftail, tail, sizeof(buftail));
	pagefault_enable();

	if (err)
		return NULL;

	perf_callchain_store(entry, buftail.lr);

	 
	if (tail >= buftail.fp)
		return NULL;

	return buftail.fp;
}