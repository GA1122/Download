void kvm_flush_remote_tlbs(struct kvm *kvm)
{
	 
	long dirty_count = smp_load_acquire(&kvm->tlbs_dirty);

	 
	if (kvm_make_all_cpus_request(kvm, KVM_REQ_TLB_FLUSH))
		++kvm->stat.remote_tlb_flush;
	cmpxchg(&kvm->tlbs_dirty, dirty_count, 0);
}
