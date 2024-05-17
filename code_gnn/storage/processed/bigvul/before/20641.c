static struct kvm_vcpu *lid_to_vcpu(struct kvm *kvm, unsigned long id,
			unsigned long eid)
{
	union ia64_lid lid;
	int i;
	struct kvm_vcpu *vcpu;

	kvm_for_each_vcpu(i, vcpu, kvm) {
		lid.val = VCPU_LID(vcpu);
		if (lid.id == id && lid.eid == eid)
			return vcpu;
	}

	return NULL;
}
