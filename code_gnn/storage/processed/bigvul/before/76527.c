int __attribute_const__ kvm_target_cpu(void)
{
	unsigned long implementor = read_cpuid_implementor();
	unsigned long part_number = read_cpuid_part_number();

	switch (implementor) {
	case ARM_CPU_IMP_ARM:
		switch (part_number) {
		case ARM_CPU_PART_AEM_V8:
			return KVM_ARM_TARGET_AEM_V8;
		case ARM_CPU_PART_FOUNDATION:
			return KVM_ARM_TARGET_FOUNDATION_V8;
		case ARM_CPU_PART_CORTEX_A53:
			return KVM_ARM_TARGET_CORTEX_A53;
		case ARM_CPU_PART_CORTEX_A57:
			return KVM_ARM_TARGET_CORTEX_A57;
		};
		break;
	case ARM_CPU_IMP_APM:
		switch (part_number) {
		case APM_CPU_PART_POTENZA:
			return KVM_ARM_TARGET_XGENE_POTENZA;
		};
		break;
	};

	 
	return KVM_ARM_TARGET_GENERIC_V8;
}