static int __kvm_set_dr(struct kvm_vcpu *vcpu, int dr, unsigned long val)
{
	switch (dr) {
	case 0 ... 3:
		vcpu->arch.db[dr] = val;
		if (!(vcpu->guest_debug & KVM_GUESTDBG_USE_HW_BP))
			vcpu->arch.eff_db[dr] = val;
		break;
	case 4:
		 
	case 6:
		if (val & 0xffffffff00000000ULL)
			return -1;  
		vcpu->arch.dr6 = (val & DR6_VOLATILE) | kvm_dr6_fixed(vcpu);
		kvm_update_dr6(vcpu);
		break;
	case 5:
		 
	default:  
		if (val & 0xffffffff00000000ULL)
			return -1;  
		vcpu->arch.dr7 = (val & DR7_VOLATILE) | DR7_FIXED_1;
		kvm_update_dr7(vcpu);
		break;
	}

	return 0;
}
