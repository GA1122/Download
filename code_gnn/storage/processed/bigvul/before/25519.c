static int __cpuinit pfault_cpu_notify(struct notifier_block *self,
				       unsigned long action, void *hcpu)
{
	struct thread_struct *thread, *next;
	struct task_struct *tsk;

	switch (action) {
	case CPU_DEAD:
	case CPU_DEAD_FROZEN:
		spin_lock_irq(&pfault_lock);
		list_for_each_entry_safe(thread, next, &pfault_list, list) {
			thread->pfault_wait = 0;
			list_del(&thread->list);
			tsk = container_of(thread, struct task_struct, thread);
			wake_up_process(tsk);
		}
		spin_unlock_irq(&pfault_lock);
		break;
	default:
		break;
	}
	return NOTIFY_OK;
}
