int kvm_arch_hardware_setup(void)
{
	int r;

	r = kvm_x86_ops->hardware_setup();
	if (r != 0)
		return r;

	if (kvm_has_tsc_control) {
		 
		u64 max = min(0x7fffffffULL,
			      __scale_tsc(kvm_max_tsc_scaling_ratio, tsc_khz));
		kvm_max_guest_tsc_khz = max;

		kvm_default_tsc_scaling_ratio = 1ULL << kvm_tsc_scaling_ratio_frac_bits;
	}

	kvm_init_msr_list();
	return 0;
}