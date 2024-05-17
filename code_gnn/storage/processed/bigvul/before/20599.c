int kvm_arch_vcpu_ioctl_get_regs(struct kvm_vcpu *vcpu, struct kvm_regs *regs)
{
	struct vpd *vpd = to_host(vcpu->kvm, vcpu->arch.vpd);
	int i;

	vcpu_load(vcpu);

	for (i = 0; i < 16; i++) {
		regs->vpd.vgr[i] = vpd->vgr[i];
		regs->vpd.vbgr[i] = vpd->vbgr[i];
	}
	for (i = 0; i < 128; i++)
		regs->vpd.vcr[i] = vpd->vcr[i];
	regs->vpd.vhpi = vpd->vhpi;
	regs->vpd.vnat = vpd->vnat;
	regs->vpd.vbnat = vpd->vbnat;
	regs->vpd.vpsr = vpd->vpsr;
	regs->vpd.vpr = vpd->vpr;

	memcpy(&regs->saved_guest, &vcpu->arch.guest, sizeof(union context));

	SAVE_REGS(mp_state);
	SAVE_REGS(vmm_rr);
	memcpy(regs->itrs, vcpu->arch.itrs, sizeof(struct thash_data) * NITRS);
	memcpy(regs->dtrs, vcpu->arch.dtrs, sizeof(struct thash_data) * NDTRS);
	SAVE_REGS(itr_regions);
	SAVE_REGS(dtr_regions);
	SAVE_REGS(tc_regions);
	SAVE_REGS(irq_check);
	SAVE_REGS(itc_check);
	SAVE_REGS(timer_check);
	SAVE_REGS(timer_pending);
	SAVE_REGS(last_itc);
	for (i = 0; i < 8; i++) {
		regs->vrr[i] = vcpu->arch.vrr[i];
		regs->ibr[i] = vcpu->arch.ibr[i];
		regs->dbr[i] = vcpu->arch.dbr[i];
	}
	for (i = 0; i < 4; i++)
		regs->insvc[i] = vcpu->arch.insvc[i];
	regs->saved_itc = vcpu->arch.itc_offset + kvm_get_itc(vcpu);
	SAVE_REGS(xtp);
	SAVE_REGS(metaphysical_rr0);
	SAVE_REGS(metaphysical_rr4);
	SAVE_REGS(metaphysical_saved_rr0);
	SAVE_REGS(metaphysical_saved_rr4);
	SAVE_REGS(fp_psr);
	SAVE_REGS(saved_gp);

	vcpu_put(vcpu);
	return 0;
}
