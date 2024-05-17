static void evmcs_sanitize_exec_ctrls(struct vmcs_config *vmcs_conf)
{
	 
	vmcs_conf->pin_based_exec_ctrl &= ~PIN_BASED_POSTED_INTR;
	vmcs_conf->cpu_based_2nd_exec_ctrl &=
		~SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY;
	vmcs_conf->cpu_based_2nd_exec_ctrl &=
		~SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES;
	vmcs_conf->cpu_based_2nd_exec_ctrl &=
		~SECONDARY_EXEC_APIC_REGISTER_VIRT;

	 
	vmcs_conf->cpu_based_2nd_exec_ctrl &= ~SECONDARY_EXEC_ENABLE_PML;

	 
	vmcs_conf->cpu_based_2nd_exec_ctrl &= ~SECONDARY_EXEC_ENABLE_VMFUNC;

	 
	vmcs_conf->cpu_based_2nd_exec_ctrl &= ~SECONDARY_EXEC_SHADOW_VMCS;

	 
	vmcs_conf->cpu_based_2nd_exec_ctrl &= ~SECONDARY_EXEC_TSC_SCALING;

	 
	vmcs_conf->cpu_based_2nd_exec_ctrl &= ~SECONDARY_EXEC_PAUSE_LOOP_EXITING;

	 
	vmcs_conf->pin_based_exec_ctrl &= ~PIN_BASED_VMX_PREEMPTION_TIMER;

	 
	vmcs_conf->vmexit_ctrl &= ~VM_EXIT_LOAD_IA32_PERF_GLOBAL_CTRL;
	vmcs_conf->vmentry_ctrl &= ~VM_ENTRY_LOAD_IA32_PERF_GLOBAL_CTRL;

	 
}
