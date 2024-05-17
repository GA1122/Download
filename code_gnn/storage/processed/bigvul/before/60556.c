int kvmppc_sanity_check(struct kvm_vcpu *vcpu)
{
	int r = false;

	 
	if (!vcpu->arch.pvr)
		goto out;

	 
	if ((vcpu->arch.cpu_type != KVM_CPU_3S_64) && vcpu->arch.papr_enabled)
		goto out;

	 
	if (!vcpu->arch.papr_enabled && is_kvmppc_hv_enabled(vcpu->kvm))
		goto out;

#ifdef CONFIG_KVM_BOOKE_HV
	if (!cpu_has_feature(CPU_FTR_EMB_HV))
		goto out;
#endif

	r = true;

out:
	vcpu->arch.sane = r;
	return r ? 0 : -EINVAL;
}
