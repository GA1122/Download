void kmsg_dump(enum kmsg_dump_reason reason)
{
	unsigned long end;
	unsigned chars;
	struct kmsg_dumper *dumper;
	const char *s1, *s2;
	unsigned long l1, l2;
	unsigned long flags;

	if ((reason > KMSG_DUMP_OOPS) && !always_kmsg_dump)
		return;

	 
	raw_spin_lock_irqsave(&logbuf_lock, flags);
	end = log_end & LOG_BUF_MASK;
	chars = logged_chars;
	raw_spin_unlock_irqrestore(&logbuf_lock, flags);

	if (chars > end) {
		s1 = log_buf + log_buf_len - chars + end;
		l1 = chars - end;

		s2 = log_buf;
		l2 = end;
	} else {
		s1 = "";
		l1 = 0;

		s2 = log_buf + end - chars;
		l2 = chars;
	}

	rcu_read_lock();
	list_for_each_entry_rcu(dumper, &dump_list, list)
		dumper->dump(dumper, reason, s1, l1, s2, l2);
	rcu_read_unlock();
}