static void nested_vmx_setup_ctls_msrs(struct vcpu_vmx *vmx)
{
	 

	 
	rdmsr(MSR_IA32_VMX_PINBASED_CTLS,
		vmx->nested.nested_vmx_pinbased_ctls_low,
		vmx->nested.nested_vmx_pinbased_ctls_high);
	vmx->nested.nested_vmx_pinbased_ctls_low |=
		PIN_BASED_ALWAYSON_WITHOUT_TRUE_MSR;
	vmx->nested.nested_vmx_pinbased_ctls_high &=
		PIN_BASED_EXT_INTR_MASK |
		PIN_BASED_NMI_EXITING |
		PIN_BASED_VIRTUAL_NMIS;
	vmx->nested.nested_vmx_pinbased_ctls_high |=
		PIN_BASED_ALWAYSON_WITHOUT_TRUE_MSR |
		PIN_BASED_VMX_PREEMPTION_TIMER;
	if (kvm_vcpu_apicv_active(&vmx->vcpu))
		vmx->nested.nested_vmx_pinbased_ctls_high |=
			PIN_BASED_POSTED_INTR;

	 
	rdmsr(MSR_IA32_VMX_EXIT_CTLS,
		vmx->nested.nested_vmx_exit_ctls_low,
		vmx->nested.nested_vmx_exit_ctls_high);
	vmx->nested.nested_vmx_exit_ctls_low =
		VM_EXIT_ALWAYSON_WITHOUT_TRUE_MSR;

	vmx->nested.nested_vmx_exit_ctls_high &=
#ifdef CONFIG_X86_64
		VM_EXIT_HOST_ADDR_SPACE_SIZE |
#endif
		VM_EXIT_LOAD_IA32_PAT | VM_EXIT_SAVE_IA32_PAT;
	vmx->nested.nested_vmx_exit_ctls_high |=
		VM_EXIT_ALWAYSON_WITHOUT_TRUE_MSR |
		VM_EXIT_LOAD_IA32_EFER | VM_EXIT_SAVE_IA32_EFER |
		VM_EXIT_SAVE_VMX_PREEMPTION_TIMER | VM_EXIT_ACK_INTR_ON_EXIT;

	if (kvm_mpx_supported())
		vmx->nested.nested_vmx_exit_ctls_high |= VM_EXIT_CLEAR_BNDCFGS;

	 
	vmx->nested.nested_vmx_true_exit_ctls_low =
		vmx->nested.nested_vmx_exit_ctls_low &
		~VM_EXIT_SAVE_DEBUG_CONTROLS;

	 
	rdmsr(MSR_IA32_VMX_ENTRY_CTLS,
		vmx->nested.nested_vmx_entry_ctls_low,
		vmx->nested.nested_vmx_entry_ctls_high);
	vmx->nested.nested_vmx_entry_ctls_low =
		VM_ENTRY_ALWAYSON_WITHOUT_TRUE_MSR;
	vmx->nested.nested_vmx_entry_ctls_high &=
#ifdef CONFIG_X86_64
		VM_ENTRY_IA32E_MODE |
#endif
		VM_ENTRY_LOAD_IA32_PAT;
	vmx->nested.nested_vmx_entry_ctls_high |=
		(VM_ENTRY_ALWAYSON_WITHOUT_TRUE_MSR | VM_ENTRY_LOAD_IA32_EFER);
	if (kvm_mpx_supported())
		vmx->nested.nested_vmx_entry_ctls_high |= VM_ENTRY_LOAD_BNDCFGS;

	 
	vmx->nested.nested_vmx_true_entry_ctls_low =
		vmx->nested.nested_vmx_entry_ctls_low &
		~VM_ENTRY_LOAD_DEBUG_CONTROLS;

	 
	rdmsr(MSR_IA32_VMX_PROCBASED_CTLS,
		vmx->nested.nested_vmx_procbased_ctls_low,
		vmx->nested.nested_vmx_procbased_ctls_high);
	vmx->nested.nested_vmx_procbased_ctls_low =
		CPU_BASED_ALWAYSON_WITHOUT_TRUE_MSR;
	vmx->nested.nested_vmx_procbased_ctls_high &=
		CPU_BASED_VIRTUAL_INTR_PENDING |
		CPU_BASED_VIRTUAL_NMI_PENDING | CPU_BASED_USE_TSC_OFFSETING |
		CPU_BASED_HLT_EXITING | CPU_BASED_INVLPG_EXITING |
		CPU_BASED_MWAIT_EXITING | CPU_BASED_CR3_LOAD_EXITING |
		CPU_BASED_CR3_STORE_EXITING |
#ifdef CONFIG_X86_64
		CPU_BASED_CR8_LOAD_EXITING | CPU_BASED_CR8_STORE_EXITING |
#endif
		CPU_BASED_MOV_DR_EXITING | CPU_BASED_UNCOND_IO_EXITING |
		CPU_BASED_USE_IO_BITMAPS | CPU_BASED_MONITOR_TRAP_FLAG |
		CPU_BASED_MONITOR_EXITING | CPU_BASED_RDPMC_EXITING |
		CPU_BASED_RDTSC_EXITING | CPU_BASED_PAUSE_EXITING |
		CPU_BASED_TPR_SHADOW | CPU_BASED_ACTIVATE_SECONDARY_CONTROLS;
	 
	vmx->nested.nested_vmx_procbased_ctls_high |=
		CPU_BASED_ALWAYSON_WITHOUT_TRUE_MSR |
		CPU_BASED_USE_MSR_BITMAPS;

	 
	vmx->nested.nested_vmx_true_procbased_ctls_low =
		vmx->nested.nested_vmx_procbased_ctls_low &
		~(CPU_BASED_CR3_LOAD_EXITING | CPU_BASED_CR3_STORE_EXITING);

	 
	rdmsr(MSR_IA32_VMX_PROCBASED_CTLS2,
		vmx->nested.nested_vmx_secondary_ctls_low,
		vmx->nested.nested_vmx_secondary_ctls_high);
	vmx->nested.nested_vmx_secondary_ctls_low = 0;
	vmx->nested.nested_vmx_secondary_ctls_high &=
		SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES |
		SECONDARY_EXEC_RDTSCP |
		SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE |
		SECONDARY_EXEC_ENABLE_VPID |
		SECONDARY_EXEC_APIC_REGISTER_VIRT |
		SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY |
		SECONDARY_EXEC_WBINVD_EXITING |
		SECONDARY_EXEC_XSAVES |
		SECONDARY_EXEC_PCOMMIT;

	if (enable_ept) {
		 
		vmx->nested.nested_vmx_secondary_ctls_high |=
			SECONDARY_EXEC_ENABLE_EPT;
		vmx->nested.nested_vmx_ept_caps = VMX_EPT_PAGE_WALK_4_BIT |
			 VMX_EPTP_WB_BIT | VMX_EPT_2MB_PAGE_BIT |
			 VMX_EPT_INVEPT_BIT;
		vmx->nested.nested_vmx_ept_caps &= vmx_capability.ept;
		 
		vmx->nested.nested_vmx_ept_caps |= VMX_EPT_EXTENT_GLOBAL_BIT;
	} else
		vmx->nested.nested_vmx_ept_caps = 0;

	 
	if (enable_vpid)
		vmx->nested.nested_vmx_vpid_caps = VMX_VPID_INVVPID_BIT |
				VMX_VPID_EXTENT_SINGLE_CONTEXT_BIT |
				VMX_VPID_EXTENT_GLOBAL_CONTEXT_BIT;
	else
		vmx->nested.nested_vmx_vpid_caps = 0;

	if (enable_unrestricted_guest)
		vmx->nested.nested_vmx_secondary_ctls_high |=
			SECONDARY_EXEC_UNRESTRICTED_GUEST;

	 
	rdmsr(MSR_IA32_VMX_MISC,
		vmx->nested.nested_vmx_misc_low,
		vmx->nested.nested_vmx_misc_high);
	vmx->nested.nested_vmx_misc_low &= VMX_MISC_SAVE_EFER_LMA;
	vmx->nested.nested_vmx_misc_low |=
		VMX_MISC_EMULATED_PREEMPTION_TIMER_RATE |
		VMX_MISC_ACTIVITY_HLT;
	vmx->nested.nested_vmx_misc_high = 0;
}