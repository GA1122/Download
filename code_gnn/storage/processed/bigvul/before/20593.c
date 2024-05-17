struct kvm_vcpu *kvm_arch_vcpu_create(struct kvm *kvm,
		unsigned int id)
{
	struct kvm_vcpu *vcpu;
	unsigned long vm_base = kvm->arch.vm_base;
	int r;
	int cpu;

	BUG_ON(sizeof(struct kvm_vcpu) > VCPU_STRUCT_SIZE/2);

	r = -EINVAL;
	if (id >= KVM_MAX_VCPUS) {
		printk(KERN_ERR"kvm: Can't configure vcpus > %ld",
				KVM_MAX_VCPUS);
		goto fail;
	}

	r = -ENOMEM;
	if (!vm_base) {
		printk(KERN_ERR"kvm: Create vcpu[%d] error!\n", id);
		goto fail;
	}
	vcpu = (struct kvm_vcpu *)(vm_base + offsetof(struct kvm_vm_data,
					vcpu_data[id].vcpu_struct));
	vcpu->kvm = kvm;

	cpu = get_cpu();
	r = vti_vcpu_setup(vcpu, id);
	put_cpu();

	if (r) {
		printk(KERN_DEBUG"kvm: vcpu_setup error!!\n");
		goto fail;
	}

	return vcpu;
fail:
	return ERR_PTR(r);
}
