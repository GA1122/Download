static int __init futex_init(void)
{
	u32 curval;
	int i;

	 
	curval = cmpxchg_futex_value_locked(NULL, 0, 0);
	if (curval == -EFAULT)
		futex_cmpxchg_enabled = 1;

	for (i = 0; i < ARRAY_SIZE(futex_queues); i++) {
		plist_head_init(&futex_queues[i].chain, &futex_queues[i].lock);
		spin_lock_init(&futex_queues[i].lock);
	}

	return 0;
}
