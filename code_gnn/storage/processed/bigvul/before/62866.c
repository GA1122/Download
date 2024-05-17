static bool access_pmcr(struct kvm_vcpu *vcpu, struct sys_reg_params *p,
			const struct sys_reg_desc *r)
{
	u64 val;

	if (!kvm_arm_pmu_v3_ready(vcpu))
		return trap_raz_wi(vcpu, p, r);

	if (pmu_access_el0_disabled(vcpu))
		return false;

	if (p->is_write) {
		 
		val = vcpu_sys_reg(vcpu, PMCR_EL0);
		val &= ~ARMV8_PMU_PMCR_MASK;
		val |= p->regval & ARMV8_PMU_PMCR_MASK;
		vcpu_sys_reg(vcpu, PMCR_EL0) = val;
		kvm_pmu_handle_pmcr(vcpu, val);
	} else {
		 
		val = vcpu_sys_reg(vcpu, PMCR_EL0)
		      & ~(ARMV8_PMU_PMCR_P | ARMV8_PMU_PMCR_C);
		p->regval = val;
	}

	return true;
}
