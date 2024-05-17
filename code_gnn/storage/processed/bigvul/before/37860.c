static void svm_handle_mce(struct vcpu_svm *svm)
{
	if (is_erratum_383()) {
		 
		pr_err("KVM: Guest triggered AMD Erratum 383\n");

		kvm_make_request(KVM_REQ_TRIPLE_FAULT, &svm->vcpu);

		return;
	}

	 
	asm volatile (
		"int $0x12\n");
	 

	return;
}
