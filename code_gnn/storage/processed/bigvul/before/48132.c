static int vmx_restore_vmx_ept_vpid_cap(struct vcpu_vmx *vmx, u64 data)
{
	u64 vmx_ept_vpid_cap;

	vmx_ept_vpid_cap = vmx_control_msr(vmx->nested.nested_vmx_ept_caps,
					   vmx->nested.nested_vmx_vpid_caps);

	 
	if (!is_bitwise_subset(vmx_ept_vpid_cap, data, -1ULL))
		return -EINVAL;

	vmx->nested.nested_vmx_ept_caps = data;
	vmx->nested.nested_vmx_vpid_caps = data >> 32;
	return 0;
}