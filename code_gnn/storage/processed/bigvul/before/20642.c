static void vcpu_deliver_ipi(struct kvm_vcpu *vcpu, uint64_t dm,
				uint64_t vector)
{
	switch (dm) {
	case SAPIC_FIXED:
		break;
	case SAPIC_NMI:
		vector = 2;
		break;
	case SAPIC_EXTINT:
		vector = 0;
		break;
	case SAPIC_INIT:
	case SAPIC_PMI:
	default:
		printk(KERN_ERR"kvm: Unimplemented Deliver reserved IPI!\n");
		return;
	}
	__apic_accept_irq(vcpu, vector);
}
