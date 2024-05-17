static void svm_set_rflags(struct kvm_vcpu *vcpu, unsigned long rflags)
{
        
	to_svm(vcpu)->vmcb->save.rflags = rflags;
}
