static int __init vmx_init(void)
{
	int r;

#if IS_ENABLED(CONFIG_HYPERV)
	 
	if (enlightened_vmcs &&
	    ms_hyperv.hints & HV_X64_ENLIGHTENED_VMCS_RECOMMENDED &&
	    (ms_hyperv.nested_features & HV_X64_ENLIGHTENED_VMCS_VERSION) >=
	    KVM_EVMCS_VERSION) {
		int cpu;

		 
		for_each_online_cpu(cpu) {
			if (!hv_get_vp_assist_page(cpu)) {
				enlightened_vmcs = false;
				break;
			}
		}

		if (enlightened_vmcs) {
			pr_info("KVM: vmx: using Hyper-V Enlightened VMCS\n");
			static_branch_enable(&enable_evmcs);
		}
	} else {
		enlightened_vmcs = false;
	}
#endif

	r = kvm_init(&vmx_x86_ops, sizeof(struct vcpu_vmx),
                     __alignof__(struct vcpu_vmx), THIS_MODULE);
	if (r)
		return r;

#ifdef CONFIG_KEXEC_CORE
	rcu_assign_pointer(crash_vmclear_loaded_vmcss,
			   crash_vmclear_local_loaded_vmcss);
#endif
	vmx_check_vmcs12_offsets();

	return 0;
}
