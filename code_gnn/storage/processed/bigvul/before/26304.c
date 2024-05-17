static void irqtime_account_process_tick(struct task_struct *p, int user_tick,
						struct rq *rq)
{
	cputime_t one_jiffy_scaled = cputime_to_scaled(cputime_one_jiffy);
	cputime64_t tmp = cputime_to_cputime64(cputime_one_jiffy);
	struct cpu_usage_stat *cpustat = &kstat_this_cpu.cpustat;

	if (irqtime_account_hi_update()) {
		cpustat->irq = cputime64_add(cpustat->irq, tmp);
	} else if (irqtime_account_si_update()) {
		cpustat->softirq = cputime64_add(cpustat->softirq, tmp);
	} else if (this_cpu_ksoftirqd() == p) {
		 
		__account_system_time(p, cputime_one_jiffy, one_jiffy_scaled,
					&cpustat->softirq);
	} else if (user_tick) {
		account_user_time(p, cputime_one_jiffy, one_jiffy_scaled);
	} else if (p == rq->idle) {
		account_idle_time(cputime_one_jiffy);
	} else if (p->flags & PF_VCPU) {  
		account_guest_time(p, cputime_one_jiffy, one_jiffy_scaled);
	} else {
		__account_system_time(p, cputime_one_jiffy, one_jiffy_scaled,
					&cpustat->system);
	}
}
