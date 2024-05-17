static void update_vttbr(struct kvm *kvm)
{
	phys_addr_t pgd_phys;
	u64 vmid;

	if (!need_new_vmid_gen(kvm))
		return;

	spin_lock(&kvm_vmid_lock);

	 
	if (!need_new_vmid_gen(kvm)) {
		spin_unlock(&kvm_vmid_lock);
		return;
	}

	 
	if (unlikely(kvm_next_vmid == 0)) {
		atomic64_inc(&kvm_vmid_gen);
		kvm_next_vmid = 1;

		 
		force_vm_exit(cpu_all_mask);
		 
		kvm_call_hyp(__kvm_flush_vm_context);
	}

	kvm->arch.vmid_gen = atomic64_read(&kvm_vmid_gen);
	kvm->arch.vmid = kvm_next_vmid;
	kvm_next_vmid++;

	 
	pgd_phys = virt_to_phys(kvm->arch.pgd);
	vmid = ((u64)(kvm->arch.vmid) << VTTBR_VMID_SHIFT) & VTTBR_VMID_MASK;
	kvm->arch.vttbr = pgd_phys & VTTBR_BADDR_MASK;
	kvm->arch.vttbr |= vmid;

	spin_unlock(&kvm_vmid_lock);
}
