static bool em_syscall_is_enabled(struct x86_emulate_ctxt *ctxt)
{
	const struct x86_emulate_ops *ops = ctxt->ops;
	u32 eax, ebx, ecx, edx;

	 
	if (ctxt->mode == X86EMUL_MODE_PROT64)
		return true;

	eax = 0x00000000;
	ecx = 0x00000000;
	ops->get_cpuid(ctxt, &eax, &ebx, &ecx, &edx);
	 
	if (ebx == X86EMUL_CPUID_VENDOR_GenuineIntel_ebx &&
	    ecx == X86EMUL_CPUID_VENDOR_GenuineIntel_ecx &&
	    edx == X86EMUL_CPUID_VENDOR_GenuineIntel_edx)
		return false;

	 
	if (ebx == X86EMUL_CPUID_VENDOR_AuthenticAMD_ebx &&
	    ecx == X86EMUL_CPUID_VENDOR_AuthenticAMD_ecx &&
	    edx == X86EMUL_CPUID_VENDOR_AuthenticAMD_edx)
		return true;

	 
	if (ebx == X86EMUL_CPUID_VENDOR_AMDisbetterI_ebx &&
	    ecx == X86EMUL_CPUID_VENDOR_AMDisbetterI_ecx &&
	    edx == X86EMUL_CPUID_VENDOR_AMDisbetterI_edx)
		return true;

	 
	return false;
}
