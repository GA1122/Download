static int vmx_smi_allowed(struct kvm_vcpu *vcpu)
{
	 
	if (to_vmx(vcpu)->nested.nested_run_pending)
		return 0;
	return 1;
}
