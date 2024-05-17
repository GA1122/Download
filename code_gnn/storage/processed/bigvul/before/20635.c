void kvm_vcpu_kick(struct kvm_vcpu *vcpu)
{
	int me;
	int cpu = vcpu->cpu;

	if (waitqueue_active(&vcpu->wq))
		wake_up_interruptible(&vcpu->wq);

	me = get_cpu();
	if (cpu != me && (unsigned) cpu < nr_cpu_ids && cpu_online(cpu))
		if (!test_and_set_bit(KVM_REQ_KICK, &vcpu->requests))
			smp_send_reschedule(cpu);
	put_cpu();
}
