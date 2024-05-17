static int vmx_check_intercept(struct kvm_vcpu *vcpu,
			       struct x86_instruction_info *info,
			       enum x86_intercept_stage stage)
{
	struct vmcs12 *vmcs12 = get_vmcs12(vcpu);
	struct x86_emulate_ctxt *ctxt = &vcpu->arch.emulate_ctxt;

	 
	if (info->intercept == x86_intercept_rdtscp &&
	    !nested_cpu_has2(vmcs12, SECONDARY_EXEC_RDTSCP)) {
		ctxt->exception.vector = UD_VECTOR;
		ctxt->exception.error_code_valid = false;
		return X86EMUL_PROPAGATE_FAULT;
	}

	 
	return X86EMUL_CONTINUE;
}