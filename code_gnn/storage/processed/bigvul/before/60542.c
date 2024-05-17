int kvmppc_emulate_mmio(struct kvm_run *run, struct kvm_vcpu *vcpu)
{
	enum emulation_result er;
	int r;

	er = kvmppc_emulate_loadstore(vcpu);
	switch (er) {
	case EMULATE_DONE:
		 
		r = RESUME_GUEST_NV;
		break;
	case EMULATE_AGAIN:
		r = RESUME_GUEST;
		break;
	case EMULATE_DO_MMIO:
		run->exit_reason = KVM_EXIT_MMIO;
		 
		 
		r = RESUME_HOST_NV;
		break;
	case EMULATE_FAIL:
	{
		u32 last_inst;

		kvmppc_get_last_inst(vcpu, INST_GENERIC, &last_inst);
		 
		pr_emerg("%s: emulation failed (%08x)\n", __func__, last_inst);
		r = RESUME_HOST;
		break;
	}
	default:
		WARN_ON(1);
		r = RESUME_GUEST;
	}

	return r;
}
