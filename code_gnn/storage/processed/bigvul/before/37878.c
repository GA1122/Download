svm_patch_hypercall(struct kvm_vcpu *vcpu, unsigned char *hypercall)
{
	 
	hypercall[0] = 0x0f;
	hypercall[1] = 0x01;
	hypercall[2] = 0xd9;
}