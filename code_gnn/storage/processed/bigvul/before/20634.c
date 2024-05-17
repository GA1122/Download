static int kvm_sn2_setup_mappings(struct kvm_vcpu *vcpu)
{
	unsigned long pte, rtc_phys_addr, map_addr;
	int slot;

	map_addr = KVM_VMM_BASE + (1UL << KVM_VMM_SHIFT);
	rtc_phys_addr = LOCAL_MMR_OFFSET | SH_RTC;
	pte = pte_val(mk_pte_phys(rtc_phys_addr, PAGE_KERNEL_UC));
	slot = ia64_itr_entry(0x3, map_addr, pte, PAGE_SHIFT);
	vcpu->arch.sn_rtc_tr_slot = slot;
	if (slot < 0) {
		printk(KERN_ERR "Mayday mayday! RTC mapping failed!\n");
		slot = 0;
	}
	return slot;
}
