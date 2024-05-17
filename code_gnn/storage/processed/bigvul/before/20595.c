 int kvm_arch_vcpu_init(struct kvm_vcpu *vcpu)
 {
 	struct kvm_vcpu *v;
	int r;
	int i;
	long itc_offset;
	struct kvm *kvm = vcpu->kvm;
	struct kvm_pt_regs *regs = vcpu_regs(vcpu);

	union context *p_ctx = &vcpu->arch.guest;
	struct kvm_vcpu *vmm_vcpu = to_guest(vcpu->kvm, vcpu);

	 
	if (IS_ERR(vmm_vcpu))
		return PTR_ERR(vmm_vcpu);

	if (kvm_vcpu_is_bsp(vcpu)) {
		vcpu->arch.mp_state = KVM_MP_STATE_RUNNABLE;

		 
		regs->cr_iip = PALE_RESET_ENTRY;

		 
		itc_offset = 0UL - kvm_get_itc(vcpu);
		for (i = 0; i < KVM_MAX_VCPUS; i++) {
			v = (struct kvm_vcpu *)((char *)vcpu +
					sizeof(struct kvm_vcpu_data) * i);
			v->arch.itc_offset = itc_offset;
			v->arch.last_itc = 0;
		}
	} else
		vcpu->arch.mp_state = KVM_MP_STATE_UNINITIALIZED;

	r = -ENOMEM;
	vcpu->arch.apic = kzalloc(sizeof(struct kvm_lapic), GFP_KERNEL);
	if (!vcpu->arch.apic)
		goto out;
	vcpu->arch.apic->vcpu = vcpu;

	p_ctx->gr[1] = 0;
	p_ctx->gr[12] = (unsigned long)((char *)vmm_vcpu + KVM_STK_OFFSET);
	p_ctx->gr[13] = (unsigned long)vmm_vcpu;
	p_ctx->psr = 0x1008522000UL;
	p_ctx->ar[40] = FPSR_DEFAULT;  
	p_ctx->caller_unat = 0;
	p_ctx->pr = 0x0;
	p_ctx->ar[36] = 0x0;  
	p_ctx->ar[19] = 0x0;  
	p_ctx->ar[18] = (unsigned long)vmm_vcpu +
				((sizeof(struct kvm_vcpu)+15) & ~15);
	p_ctx->ar[64] = 0x0;  
	p_ctx->cr[0] = 0x7e04UL;
	p_ctx->cr[2] = (unsigned long)kvm_vmm_info->vmm_ivt;
	p_ctx->cr[8] = 0x3c;

	 
	p_ctx->rr[0] = 0x30;
	p_ctx->rr[1] = 0x30;
	p_ctx->rr[2] = 0x30;
	p_ctx->rr[3] = 0x30;
	p_ctx->rr[4] = 0x30;
	p_ctx->rr[5] = 0x30;
	p_ctx->rr[7] = 0x30;

	 
	p_ctx->br[0] = *(unsigned long *)kvm_vmm_info->vmm_entry;

	vcpu->arch.vmm_rr = kvm->arch.vmm_init_rr;
	vcpu->arch.metaphysical_rr0 = kvm->arch.metaphysical_rr0;
	vcpu->arch.metaphysical_rr4 = kvm->arch.metaphysical_rr4;

	hrtimer_init(&vcpu->arch.hlt_timer, CLOCK_MONOTONIC, HRTIMER_MODE_ABS);
	vcpu->arch.hlt_timer.function = hlt_timer_fn;

	vcpu->arch.last_run_cpu = -1;
	vcpu->arch.vpd = (struct vpd *)VPD_BASE(vcpu->vcpu_id);
	vcpu->arch.vsa_base = kvm_vsa_base;
	vcpu->arch.__gp = kvm_vmm_gp;
	vcpu->arch.dirty_log_lock_pa = __pa(&kvm->arch.dirty_log_lock);
	vcpu->arch.vhpt.hash = (struct thash_data *)VHPT_BASE(vcpu->vcpu_id);
	vcpu->arch.vtlb.hash = (struct thash_data *)VTLB_BASE(vcpu->vcpu_id);
	init_ptce_info(vcpu);

	r = 0;
out:
	return r;
}