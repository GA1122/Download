void kvm_apic_write_nodecode(struct kvm_vcpu *vcpu, u32 offset)
{
	u32 val = 0;

	 
	offset &= 0xff0;

	apic_reg_read(vcpu->arch.apic, offset, 4, &val);

	 
	apic_reg_write(vcpu->arch.apic, offset, val);
}
