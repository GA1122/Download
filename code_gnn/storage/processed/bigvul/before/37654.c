 static __init void nested_vmx_setup_ctls_msrs(void)
 {
 	 

	 
	rdmsr(MSR_IA32_VMX_PINBASED_CTLS,
	      nested_vmx_pinbased_ctls_low, nested_vmx_pinbased_ctls_high);
	 
	nested_vmx_pinbased_ctls_low |= PIN_BASED_ALWAYSON_WITHOUT_TRUE_MSR;
	nested_vmx_pinbased_ctls_high &= PIN_BASED_EXT_INTR_MASK |
		PIN_BASED_NMI_EXITING | PIN_BASED_VIRTUAL_NMIS |
		PIN_BASED_VMX_PREEMPTION_TIMER;
	nested_vmx_pinbased_ctls_high |= PIN_BASED_ALWAYSON_WITHOUT_TRUE_MSR;

	 
	nested_vmx_exit_ctls_low = VM_EXIT_ALWAYSON_WITHOUT_TRUE_MSR;
	 
#ifdef CONFIG_X86_64
	nested_vmx_exit_ctls_high = VM_EXIT_HOST_ADDR_SPACE_SIZE;
#else
	nested_vmx_exit_ctls_high = 0;
#endif
	nested_vmx_exit_ctls_high |= (VM_EXIT_ALWAYSON_WITHOUT_TRUE_MSR |
				      VM_EXIT_LOAD_IA32_EFER);

	 
	rdmsr(MSR_IA32_VMX_ENTRY_CTLS,
		nested_vmx_entry_ctls_low, nested_vmx_entry_ctls_high);
	 
	nested_vmx_entry_ctls_low = VM_ENTRY_ALWAYSON_WITHOUT_TRUE_MSR;
	nested_vmx_entry_ctls_high &=
		VM_ENTRY_LOAD_IA32_PAT | VM_ENTRY_IA32E_MODE;
	nested_vmx_entry_ctls_high |= (VM_ENTRY_ALWAYSON_WITHOUT_TRUE_MSR |
				       VM_ENTRY_LOAD_IA32_EFER);
	 
	rdmsr(MSR_IA32_VMX_PROCBASED_CTLS,
		nested_vmx_procbased_ctls_low, nested_vmx_procbased_ctls_high);
	nested_vmx_procbased_ctls_low = 0;
	nested_vmx_procbased_ctls_high &=
		CPU_BASED_VIRTUAL_INTR_PENDING | CPU_BASED_USE_TSC_OFFSETING |
		CPU_BASED_HLT_EXITING | CPU_BASED_INVLPG_EXITING |
		CPU_BASED_MWAIT_EXITING | CPU_BASED_CR3_LOAD_EXITING |
		CPU_BASED_CR3_STORE_EXITING |
#ifdef CONFIG_X86_64
		CPU_BASED_CR8_LOAD_EXITING | CPU_BASED_CR8_STORE_EXITING |
#endif
		CPU_BASED_MOV_DR_EXITING | CPU_BASED_UNCOND_IO_EXITING |
		CPU_BASED_USE_IO_BITMAPS | CPU_BASED_MONITOR_EXITING |
		CPU_BASED_RDPMC_EXITING | CPU_BASED_RDTSC_EXITING |
		CPU_BASED_PAUSE_EXITING |
		CPU_BASED_ACTIVATE_SECONDARY_CONTROLS;
	 
	nested_vmx_procbased_ctls_high |= CPU_BASED_USE_MSR_BITMAPS;

	 
	rdmsr(MSR_IA32_VMX_PROCBASED_CTLS2,
		nested_vmx_secondary_ctls_low, nested_vmx_secondary_ctls_high);
	nested_vmx_secondary_ctls_low = 0;
	nested_vmx_secondary_ctls_high &=
		SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES |
		SECONDARY_EXEC_WBINVD_EXITING;

	 
	rdmsr(MSR_IA32_VMX_MISC, nested_vmx_misc_low, nested_vmx_misc_high);
	nested_vmx_misc_low &= VMX_MISC_PREEMPTION_TIMER_RATE_MASK |
		VMX_MISC_SAVE_EFER_LMA;
	nested_vmx_misc_high = 0;
}
