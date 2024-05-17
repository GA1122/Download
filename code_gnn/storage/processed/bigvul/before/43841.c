static int apply_microcode_early(struct ucode_cpu_info *uci, bool early)
{
	struct microcode_intel *mc_intel;
	unsigned int val[2];

	mc_intel = uci->mc;
	if (mc_intel == NULL)
		return 0;

	 
	native_wrmsr(MSR_IA32_UCODE_WRITE,
	      (unsigned long) mc_intel->bits,
	      (unsigned long) mc_intel->bits >> 16 >> 16);
	native_wrmsr(MSR_IA32_UCODE_REV, 0, 0);

	 
	sync_core();

	 
	native_rdmsr(MSR_IA32_UCODE_REV, val[0], val[1]);
	if (val[1] != mc_intel->hdr.rev)
		return -1;

#ifdef CONFIG_X86_64
	 
	flush_tlb_early();
#endif
	uci->cpu_sig.rev = val[1];

	if (early)
		print_ucode(uci);
	else
		print_ucode_info(uci, mc_intel->hdr.date);

	return 0;
}
