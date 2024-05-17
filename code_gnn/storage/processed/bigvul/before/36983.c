static __init int adjust_vmx_controls(u32 ctl_min, u32 ctl_opt,
				      u32 msr, u32 *result)
{
	u32 vmx_msr_low, vmx_msr_high;
	u32 ctl = ctl_min | ctl_opt;

	rdmsr(msr, vmx_msr_low, vmx_msr_high);

	ctl &= vmx_msr_high;  
	ctl |= vmx_msr_low;   

	 
	if (ctl_min & ~ctl)
		return -EIO;

	*result = ctl;
	return 0;
}
