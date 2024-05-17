int printk_sched(const char *fmt, ...)
{
	unsigned long flags;
	va_list args;
	char *buf;
	int r;

	local_irq_save(flags);
	buf = __get_cpu_var(printk_sched_buf);

	va_start(args, fmt);
	r = vsnprintf(buf, PRINTK_BUF_SIZE, fmt, args);
	va_end(args);

	__this_cpu_or(printk_pending, PRINTK_PENDING_SCHED);
	local_irq_restore(flags);

	return r;
}
