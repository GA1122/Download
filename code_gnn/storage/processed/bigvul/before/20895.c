u64 vcpu_tsc_khz(struct kvm_vcpu *vcpu)
{
	if (vcpu->arch.virtual_tsc_khz)
		return vcpu->arch.virtual_tsc_khz;
	else
		return __this_cpu_read(cpu_tsc_khz);
}
