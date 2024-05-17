static void kvm_smm_changed(struct kvm_vcpu *vcpu)
{
	if (!(vcpu->arch.hflags & HF_SMM_MASK)) {
		 
		trace_kvm_enter_smm(vcpu->vcpu_id, vcpu->arch.smbase, false);

		if (unlikely(vcpu->arch.smi_pending)) {
			kvm_make_request(KVM_REQ_SMI, vcpu);
			vcpu->arch.smi_pending = 0;
		} else {
			 
			kvm_make_request(KVM_REQ_EVENT, vcpu);
		}
	}

	kvm_mmu_reset_context(vcpu);
}
