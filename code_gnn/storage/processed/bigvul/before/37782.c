static int nested_svm_exit_special(struct vcpu_svm *svm)
{
	u32 exit_code = svm->vmcb->control.exit_code;

	switch (exit_code) {
	case SVM_EXIT_INTR:
	case SVM_EXIT_NMI:
	case SVM_EXIT_EXCP_BASE + MC_VECTOR:
		return NESTED_EXIT_HOST;
	case SVM_EXIT_NPF:
		 
		if (npt_enabled)
			return NESTED_EXIT_HOST;
		break;
	case SVM_EXIT_EXCP_BASE + PF_VECTOR:
		 
		if (!npt_enabled && svm->apf_reason == 0)
			return NESTED_EXIT_HOST;
		break;
	case SVM_EXIT_EXCP_BASE + NM_VECTOR:
		nm_interception(svm);
		break;
	default:
		break;
	}

	return NESTED_EXIT_CONTINUE;
}
