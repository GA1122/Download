static void svm_set_supported_cpuid(u32 func, struct kvm_cpuid_entry2 *entry)
{
	switch (func) {
	case 0x80000001:
		if (nested)
			entry->ecx |= (1 << 2);  
		break;
	case 0x8000000A:
		entry->eax = 1;  
		entry->ebx = 8;  
		entry->ecx = 0;  
		entry->edx = 0;  

		 
		if (boot_cpu_has(X86_FEATURE_NRIPS))
			entry->edx |= SVM_FEATURE_NRIP;

		 
		if (npt_enabled)
			entry->edx |= SVM_FEATURE_NPT;

		break;
	}
}
