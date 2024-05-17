static int vcpu_interrupt_line(struct kvm_vcpu *vcpu, int number, bool level)
{
	int bit_index;
	bool set;
	unsigned long *ptr;

	if (number == KVM_ARM_IRQ_CPU_IRQ)
		bit_index = __ffs(HCR_VI);
	else  
		bit_index = __ffs(HCR_VF);

	ptr = (unsigned long *)&vcpu->arch.irq_lines;
	if (level)
		set = test_and_set_bit(bit_index, ptr);
	else
		set = test_and_clear_bit(bit_index, ptr);

	 
	if (set == level)
		return 0;

	 
	kvm_vcpu_kick(vcpu);

	return 0;
}
