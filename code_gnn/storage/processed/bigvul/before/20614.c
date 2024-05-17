long kvm_arch_vm_ioctl(struct file *filp,
		unsigned int ioctl, unsigned long arg)
{
	struct kvm *kvm = filp->private_data;
	void __user *argp = (void __user *)arg;
	int r = -ENOTTY;

	switch (ioctl) {
	case KVM_SET_MEMORY_REGION: {
		struct kvm_memory_region kvm_mem;
		struct kvm_userspace_memory_region kvm_userspace_mem;

		r = -EFAULT;
		if (copy_from_user(&kvm_mem, argp, sizeof kvm_mem))
			goto out;
		kvm_userspace_mem.slot = kvm_mem.slot;
		kvm_userspace_mem.flags = kvm_mem.flags;
		kvm_userspace_mem.guest_phys_addr =
					kvm_mem.guest_phys_addr;
		kvm_userspace_mem.memory_size = kvm_mem.memory_size;
		r = kvm_vm_ioctl_set_memory_region(kvm,
					&kvm_userspace_mem, 0);
		if (r)
			goto out;
		break;
		}
	case KVM_CREATE_IRQCHIP:
		r = -EFAULT;
		r = kvm_ioapic_init(kvm);
		if (r)
			goto out;
		r = kvm_setup_default_irq_routing(kvm);
		if (r) {
			mutex_lock(&kvm->slots_lock);
			kvm_ioapic_destroy(kvm);
			mutex_unlock(&kvm->slots_lock);
			goto out;
		}
		break;
	case KVM_IRQ_LINE_STATUS:
	case KVM_IRQ_LINE: {
		struct kvm_irq_level irq_event;

		r = -EFAULT;
		if (copy_from_user(&irq_event, argp, sizeof irq_event))
			goto out;
		r = -ENXIO;
		if (irqchip_in_kernel(kvm)) {
			__s32 status;
			status = kvm_set_irq(kvm, KVM_USERSPACE_IRQ_SOURCE_ID,
				    irq_event.irq, irq_event.level);
			if (ioctl == KVM_IRQ_LINE_STATUS) {
				r = -EFAULT;
				irq_event.status = status;
				if (copy_to_user(argp, &irq_event,
							sizeof irq_event))
					goto out;
			}
			r = 0;
		}
		break;
		}
	case KVM_GET_IRQCHIP: {
		 
		struct kvm_irqchip chip;

		r = -EFAULT;
		if (copy_from_user(&chip, argp, sizeof chip))
				goto out;
		r = -ENXIO;
		if (!irqchip_in_kernel(kvm))
			goto out;
		r = kvm_vm_ioctl_get_irqchip(kvm, &chip);
		if (r)
			goto out;
		r = -EFAULT;
		if (copy_to_user(argp, &chip, sizeof chip))
				goto out;
		r = 0;
		break;
		}
	case KVM_SET_IRQCHIP: {
		 
		struct kvm_irqchip chip;

		r = -EFAULT;
		if (copy_from_user(&chip, argp, sizeof chip))
				goto out;
		r = -ENXIO;
		if (!irqchip_in_kernel(kvm))
			goto out;
		r = kvm_vm_ioctl_set_irqchip(kvm, &chip);
		if (r)
			goto out;
		r = 0;
		break;
		}
	default:
		;
	}
out:
	return r;
}
