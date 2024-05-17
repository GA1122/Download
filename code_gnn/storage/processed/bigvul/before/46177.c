int __sb_start_write(struct super_block *sb, int level, bool wait)
{
retry:
	if (unlikely(sb->s_writers.frozen >= level)) {
		if (!wait)
			return 0;
		wait_event(sb->s_writers.wait_unfrozen,
			   sb->s_writers.frozen < level);
	}

#ifdef CONFIG_LOCKDEP
	acquire_freeze_lock(sb, level, !wait, _RET_IP_);
#endif
	percpu_counter_inc(&sb->s_writers.counter[level-1]);
	 
	smp_mb();
	if (unlikely(sb->s_writers.frozen >= level)) {
		__sb_end_write(sb, level);
		goto retry;
	}
	return 1;
}