static unsigned long kvm_get_itc(struct kvm_vcpu *vcpu)
{
#if defined(CONFIG_IA64_SGI_SN2) || defined(CONFIG_IA64_GENERIC)
	if (vcpu->kvm->arch.is_sn2)
		return rtc_time();
	else
#endif
		return ia64_getreg(_IA64_REG_AR_ITC);
}
