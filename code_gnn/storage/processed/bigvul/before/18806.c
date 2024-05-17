u64 snmp_fold_field64(void __percpu *mib[], int offt, size_t syncp_offset)
{
	u64 res = 0;
	int cpu;

	for_each_possible_cpu(cpu) {
		void *bhptr, *userptr;
		struct u64_stats_sync *syncp;
		u64 v_bh, v_user;
		unsigned int start;

		 
		bhptr = per_cpu_ptr(SNMP_STAT_BHPTR(mib), cpu);
		syncp = (struct u64_stats_sync *)(bhptr + syncp_offset);
		do {
			start = u64_stats_fetch_begin_bh(syncp);
			v_bh = *(((u64 *) bhptr) + offt);
		} while (u64_stats_fetch_retry_bh(syncp, start));

		 
		userptr = per_cpu_ptr(SNMP_STAT_USRPTR(mib), cpu);
		syncp = (struct u64_stats_sync *)(userptr + syncp_offset);
		do {
			start = u64_stats_fetch_begin(syncp);
			v_user = *(((u64 *) userptr) + offt);
		} while (u64_stats_fetch_retry(syncp, start));

		res += v_bh + v_user;
	}
	return res;
}