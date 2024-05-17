static void kvm_arch_set_tsc_khz(struct kvm *kvm, u32 this_tsc_khz)
{
	 
	kvm_get_time_scale(this_tsc_khz, NSEC_PER_SEC / 1000,
			   &kvm->arch.virtual_tsc_shift,
			   &kvm->arch.virtual_tsc_mult);
	kvm->arch.virtual_tsc_khz = this_tsc_khz;
}
