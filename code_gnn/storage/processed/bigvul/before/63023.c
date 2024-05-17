static bool valid_ept_address(struct kvm_vcpu *vcpu, u64 address)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	int maxphyaddr = cpuid_maxphyaddr(vcpu);

	 
	switch (address & VMX_EPTP_MT_MASK) {
	case VMX_EPTP_MT_UC:
		if (!(vmx->nested.nested_vmx_ept_caps & VMX_EPTP_UC_BIT))
			return false;
		break;
	case VMX_EPTP_MT_WB:
		if (!(vmx->nested.nested_vmx_ept_caps & VMX_EPTP_WB_BIT))
			return false;
		break;
	default:
		return false;
	}

	 
	if ((address & VMX_EPTP_PWL_MASK) != VMX_EPTP_PWL_4)
		return false;

	 
	if (address >> maxphyaddr || ((address >> 7) & 0x1f))
		return false;

	 
	if (address & VMX_EPTP_AD_ENABLE_BIT) {
		if (!(vmx->nested.nested_vmx_ept_caps & VMX_EPT_AD_BIT))
			return false;
	}

	return true;
}
