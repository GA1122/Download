static unsigned long nested_ept_get_cr3(struct kvm_vcpu *vcpu)
{
	 
	return get_vmcs12(vcpu)->ept_pointer;
}
