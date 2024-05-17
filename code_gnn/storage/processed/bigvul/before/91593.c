void kvm_vcpu_uninit(struct kvm_vcpu *vcpu)
{
	 
	put_pid(rcu_dereference_protected(vcpu->pid, 1));
	kvm_arch_vcpu_uninit(vcpu);
	free_page((unsigned long)vcpu->run);
}
