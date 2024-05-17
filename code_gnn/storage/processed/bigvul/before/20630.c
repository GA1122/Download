static void kvm_purge_vmm_mapping(struct kvm_vcpu *vcpu)
{
	struct kvm *kvm = vcpu->kvm;
	ia64_ptr_entry(0x3, vcpu->arch.vmm_tr_slot);
	ia64_ptr_entry(0x3, vcpu->arch.vm_tr_slot);
#if defined(CONFIG_IA64_SGI_SN2) || defined(CONFIG_IA64_GENERIC)
	if (kvm->arch.is_sn2)
		ia64_ptr_entry(0x3, vcpu->arch.sn_rtc_tr_slot);
#endif
}
