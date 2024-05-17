static void kvm_init_tsc_catchup(struct kvm_vcpu *vcpu, u32 this_tsc_khz)
{
	 
	kvm_get_time_scale(this_tsc_khz, NSEC_PER_SEC / 1000,
			   &vcpu->arch.tsc_catchup_shift,
			   &vcpu->arch.tsc_catchup_mult);
}
