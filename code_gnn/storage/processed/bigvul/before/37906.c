static int svm_set_vm_cr(struct kvm_vcpu *vcpu, u64 data)
{
	struct vcpu_svm *svm = to_svm(vcpu);
	int svm_dis, chg_mask;

	if (data & ~SVM_VM_CR_VALID_MASK)
		return 1;

	chg_mask = SVM_VM_CR_VALID_MASK;

	if (svm->nested.vm_cr_msr & SVM_VM_CR_SVM_DIS_MASK)
		chg_mask &= ~(SVM_VM_CR_SVM_LOCK_MASK | SVM_VM_CR_SVM_DIS_MASK);

	svm->nested.vm_cr_msr &= ~chg_mask;
	svm->nested.vm_cr_msr |= (data & chg_mask);

	svm_dis = svm->nested.vm_cr_msr & SVM_VM_CR_SVM_DIS_MASK;

	 
	if (svm_dis && (vcpu->arch.efer & EFER_SVME))
		return 1;

	return 0;
}