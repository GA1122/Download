static void vmx_vcpu_load(struct kvm_vcpu *vcpu, int cpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	u64 phys_addr = __pa(per_cpu(vmxarea, cpu));

	if (!vmm_exclusive)
		kvm_cpu_vmxon(phys_addr);
	else if (vmx->loaded_vmcs->cpu != cpu)
		loaded_vmcs_clear(vmx->loaded_vmcs);

	if (per_cpu(current_vmcs, cpu) != vmx->loaded_vmcs->vmcs) {
		per_cpu(current_vmcs, cpu) = vmx->loaded_vmcs->vmcs;
		vmcs_load(vmx->loaded_vmcs->vmcs);
	}

	if (vmx->loaded_vmcs->cpu != cpu) {
		struct desc_ptr *gdt = this_cpu_ptr(&host_gdt);
		unsigned long sysenter_esp;

		kvm_make_request(KVM_REQ_TLB_FLUSH, vcpu);
		local_irq_disable();
		crash_disable_local_vmclear(cpu);

		 
		smp_rmb();

		list_add(&vmx->loaded_vmcs->loaded_vmcss_on_cpu_link,
			 &per_cpu(loaded_vmcss_on_cpu, cpu));
		crash_enable_local_vmclear(cpu);
		local_irq_enable();

		 
		vmcs_writel(HOST_TR_BASE, kvm_read_tr_base());  
		vmcs_writel(HOST_GDTR_BASE, gdt->address);    

		rdmsrl(MSR_IA32_SYSENTER_ESP, sysenter_esp);
		vmcs_writel(HOST_IA32_SYSENTER_ESP, sysenter_esp);  
		vmx->loaded_vmcs->cpu = cpu;
	}
}