static void vmcs_set_secondary_exec_control(u32 new_ctl)
{
	 
	u32 mask =
		SECONDARY_EXEC_SHADOW_VMCS |
		SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE |
		SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES |
		SECONDARY_EXEC_DESC;

	u32 cur_ctl = vmcs_read32(SECONDARY_VM_EXEC_CONTROL);

	vmcs_write32(SECONDARY_VM_EXEC_CONTROL,
		     (new_ctl & ~mask) | (cur_ctl & mask));
}
