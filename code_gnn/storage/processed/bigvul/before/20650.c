static int __kvm_set_dr(struct kvm_vcpu *vcpu, int dr, unsigned long val)
{
	switch (dr) {
	case 0 ... 3:
		vcpu->arch.db[dr] = val;
		if (!(vcpu->guest_debug & KVM_GUESTDBG_USE_HW_BP))
			vcpu->arch.eff_db[dr] = val;
		break;
	case 4:
		if (kvm_read_cr4_bits(vcpu, X86_CR4_DE))
			return 1;  
		 
	case 6:
		if (val & 0xffffffff00000000ULL)
			return -1;  
		vcpu->arch.dr6 = (val & DR6_VOLATILE) | DR6_FIXED_1;
		break;
	case 5:
		if (kvm_read_cr4_bits(vcpu, X86_CR4_DE))
			return 1;  
		 
	default:  
		if (val & 0xffffffff00000000ULL)
			return -1;  
		vcpu->arch.dr7 = (val & DR7_VOLATILE) | DR7_FIXED_1;
		if (!(vcpu->guest_debug & KVM_GUESTDBG_USE_HW_BP)) {
			kvm_x86_ops->set_dr7(vcpu, vcpu->arch.dr7);
			vcpu->arch.switch_db_regs = (val & DR7_BP_EN_MASK);
		}
		break;
	}

	return 0;
}