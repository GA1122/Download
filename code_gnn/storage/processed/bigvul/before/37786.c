static void nested_svm_inject_npf_exit(struct kvm_vcpu *vcpu,
				       struct x86_exception *fault)
{
	struct vcpu_svm *svm = to_svm(vcpu);

	if (svm->vmcb->control.exit_code != SVM_EXIT_NPF) {
		 
		svm->vmcb->control.exit_code = SVM_EXIT_NPF;
		svm->vmcb->control.exit_code_hi = 0;
		svm->vmcb->control.exit_info_1 = (1ULL << 32);
		svm->vmcb->control.exit_info_2 = fault->address;
	}

	svm->vmcb->control.exit_info_1 &= ~0xffffffffULL;
	svm->vmcb->control.exit_info_1 |= fault->error_code;

	 
	if (svm->vmcb->control.exit_info_1 & (2ULL << 32))
		svm->vmcb->control.exit_info_1 &= ~1;

	nested_svm_vmexit(svm);
}