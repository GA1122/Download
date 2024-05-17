static void pfault_interrupt(unsigned int ext_int_code,
			     unsigned int param32, unsigned long param64)
{
	struct task_struct *tsk;
	__u16 subcode;
	pid_t pid;

	 
	subcode = ext_int_code >> 16;
	if ((subcode & 0xff00) != __SUBCODE_MASK)
		return;
	kstat_cpu(smp_processor_id()).irqs[EXTINT_PFL]++;
	if (subcode & 0x0080) {
		 
		pid = sizeof(void *) == 4 ? param32 : param64;
		rcu_read_lock();
		tsk = find_task_by_pid_ns(pid, &init_pid_ns);
		if (tsk)
			get_task_struct(tsk);
		rcu_read_unlock();
		if (!tsk)
			return;
	} else {
		tsk = current;
	}
	spin_lock(&pfault_lock);
	if (subcode & 0x0080) {
		 
		if (tsk->thread.pfault_wait == 1) {
			 
			tsk->thread.pfault_wait = 0;
			list_del(&tsk->thread.list);
			wake_up_process(tsk);
		} else {
			 
			tsk->thread.pfault_wait = -1;
		}
		put_task_struct(tsk);
	} else {
		 
		if (tsk->thread.pfault_wait == -1) {
			 
			tsk->thread.pfault_wait = 0;
		} else {
			 
			tsk->thread.pfault_wait = 1;
			list_add(&tsk->thread.list, &pfault_list);
			set_task_state(tsk, TASK_UNINTERRUPTIBLE);
			set_tsk_need_resched(tsk);
		}
	}
	spin_unlock(&pfault_lock);
}
