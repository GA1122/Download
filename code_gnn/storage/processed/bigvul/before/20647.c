static int vti_init_vpd(struct kvm_vcpu *vcpu)
{
	int i;
	union cpuid3_t cpuid3;
	struct vpd *vpd = to_host(vcpu->kvm, vcpu->arch.vpd);

	if (IS_ERR(vpd))
		return PTR_ERR(vpd);

	 
	for (i = 0; i < 5; i++)
		vpd->vcpuid[i] = ia64_get_cpuid(i);

	 
	cpuid3.value = vpd->vcpuid[3];
	cpuid3.number = 4;	 
	vpd->vcpuid[3] = cpuid3.value;

	 
	vpd->vac.a_from_int_cr = 1;
	vpd->vac.a_to_int_cr = 1;
	vpd->vac.a_from_psr = 1;
	vpd->vac.a_from_cpuid = 1;
	vpd->vac.a_cover = 1;
	vpd->vac.a_bsw = 1;
	vpd->vac.a_int = 1;
	vpd->vdc.d_vmsw = 1;

	 
	vpd->virt_env_vaddr = KVM_VM_BUFFER_BASE;

	return 0;
}
