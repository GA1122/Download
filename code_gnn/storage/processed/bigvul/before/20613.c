void kvm_arch_vcpu_uninit(struct kvm_vcpu *vcpu)
{

	hrtimer_cancel(&vcpu->arch.hlt_timer);
	kfree(vcpu->arch.apic);
}
