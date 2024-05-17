int kvm_lapic_find_highest_irr(struct kvm_vcpu *vcpu)
{
	int highest_irr;

	 
	if (!kvm_vcpu_has_lapic(vcpu))
		return 0;
	highest_irr = apic_find_highest_irr(vcpu->arch.apic);

	return highest_irr;
}
