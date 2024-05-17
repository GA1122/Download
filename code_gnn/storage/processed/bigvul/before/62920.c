static void reset_pmcr(struct kvm_vcpu *vcpu, const struct sys_reg_desc *r)
{
	u64 pmcr, val;

	pmcr = read_sysreg(pmcr_el0);
	 
	val = ((pmcr & ~ARMV8_PMU_PMCR_MASK)
	       | (ARMV8_PMU_PMCR_MASK & 0xdecafbad)) & (~ARMV8_PMU_PMCR_E);
	vcpu_sys_reg(vcpu, PMCR_EL0) = val;
}
