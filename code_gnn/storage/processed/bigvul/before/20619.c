static void kvm_flush_icache(unsigned long start, unsigned long len)
{
	int l;

	for (l = 0; l < (len + 32); l += 32)
		ia64_fc((void *)(start + l));

	ia64_sync_i();
	ia64_srlz_i();
}
