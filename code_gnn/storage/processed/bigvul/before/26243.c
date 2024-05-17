void __account_system_time(struct task_struct *p, cputime_t cputime,
			cputime_t cputime_scaled, cputime64_t *target_cputime64)
{
	cputime64_t tmp = cputime_to_cputime64(cputime);

	 
	p->stime = cputime_add(p->stime, cputime);
	p->stimescaled = cputime_add(p->stimescaled, cputime_scaled);
	account_group_system_time(p, cputime);

	 
	*target_cputime64 = cputime64_add(*target_cputime64, tmp);
	cpuacct_update_stats(p, CPUACCT_STAT_SYSTEM, cputime);

	 
	acct_update_integrals(p);
}
