static int skinit_interception(struct vcpu_svm *svm)
{
	trace_kvm_skinit(svm->vmcb->save.rip, kvm_register_read(&svm->vcpu, VCPU_REGS_RAX));

	kvm_queue_exception(&svm->vcpu, UD_VECTOR);
	return 1;
}