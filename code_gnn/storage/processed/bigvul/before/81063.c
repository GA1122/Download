static void vmx_save_host_state(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
#ifdef CONFIG_X86_64
	int cpu = raw_smp_processor_id();
#endif
	int i;

	if (vmx->host_state.loaded)
		return;

	vmx->host_state.loaded = 1;
	 
	vmx->host_state.ldt_sel = kvm_read_ldt();
	vmx->host_state.gs_ldt_reload_needed = vmx->host_state.ldt_sel;

#ifdef CONFIG_X86_64
	save_fsgs_for_kvm();
	vmx->host_state.fs_sel = current->thread.fsindex;
	vmx->host_state.gs_sel = current->thread.gsindex;
#else
	savesegment(fs, vmx->host_state.fs_sel);
	savesegment(gs, vmx->host_state.gs_sel);
#endif
	if (!(vmx->host_state.fs_sel & 7)) {
		vmcs_write16(HOST_FS_SELECTOR, vmx->host_state.fs_sel);
		vmx->host_state.fs_reload_needed = 0;
	} else {
		vmcs_write16(HOST_FS_SELECTOR, 0);
		vmx->host_state.fs_reload_needed = 1;
	}
	if (!(vmx->host_state.gs_sel & 7))
		vmcs_write16(HOST_GS_SELECTOR, vmx->host_state.gs_sel);
	else {
		vmcs_write16(HOST_GS_SELECTOR, 0);
		vmx->host_state.gs_ldt_reload_needed = 1;
	}

#ifdef CONFIG_X86_64
	savesegment(ds, vmx->host_state.ds_sel);
	savesegment(es, vmx->host_state.es_sel);

	vmcs_writel(HOST_FS_BASE, current->thread.fsbase);
	vmcs_writel(HOST_GS_BASE, cpu_kernelmode_gs_base(cpu));

	vmx->msr_host_kernel_gs_base = current->thread.gsbase;
	if (is_long_mode(&vmx->vcpu))
		wrmsrl(MSR_KERNEL_GS_BASE, vmx->msr_guest_kernel_gs_base);
#else
	vmcs_writel(HOST_FS_BASE, segment_base(vmx->host_state.fs_sel));
	vmcs_writel(HOST_GS_BASE, segment_base(vmx->host_state.gs_sel));
#endif
	if (boot_cpu_has(X86_FEATURE_MPX))
		rdmsrl(MSR_IA32_BNDCFGS, vmx->host_state.msr_host_bndcfgs);
	for (i = 0; i < vmx->save_nmsrs; ++i)
		kvm_set_shared_msr(vmx->guest_msrs[i].index,
				   vmx->guest_msrs[i].data,
				   vmx->guest_msrs[i].mask);
}