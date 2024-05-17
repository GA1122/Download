void kvm_arch_flush_shadow(struct kvm *kvm)
{
	kvm_flush_remote_tlbs(kvm);
}
