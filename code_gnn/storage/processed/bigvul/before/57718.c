long kvm_arch_vm_ioctl(struct file *filp,
		       unsigned int ioctl, unsigned long arg)
{
	struct kvm *kvm = filp->private_data;
	void __user *argp = (void __user *)arg;
	int r = -ENOTTY;
	 
	union {
		struct kvm_pit_state ps;
		struct kvm_pit_state2 ps2;
		struct kvm_pit_config pit_config;
	} u;

	switch (ioctl) {
	case KVM_SET_TSS_ADDR:
		r = kvm_vm_ioctl_set_tss_addr(kvm, arg);
		break;
	case KVM_SET_IDENTITY_MAP_ADDR: {
		u64 ident_addr;

		r = -EFAULT;
		if (copy_from_user(&ident_addr, argp, sizeof ident_addr))
			goto out;
		r = kvm_vm_ioctl_set_identity_map_addr(kvm, ident_addr);
		break;
	}
	case KVM_SET_NR_MMU_PAGES:
		r = kvm_vm_ioctl_set_nr_mmu_pages(kvm, arg);
		break;
	case KVM_GET_NR_MMU_PAGES:
		r = kvm_vm_ioctl_get_nr_mmu_pages(kvm);
		break;
	case KVM_CREATE_IRQCHIP: {
		struct kvm_pic *vpic;

		mutex_lock(&kvm->lock);
		r = -EEXIST;
		if (kvm->arch.vpic)
			goto create_irqchip_unlock;
		r = -EINVAL;
		if (atomic_read(&kvm->online_vcpus))
			goto create_irqchip_unlock;
		r = -ENOMEM;
		vpic = kvm_create_pic(kvm);
		if (vpic) {
			r = kvm_ioapic_init(kvm);
			if (r) {
				mutex_lock(&kvm->slots_lock);
				kvm_destroy_pic(vpic);
				mutex_unlock(&kvm->slots_lock);
				goto create_irqchip_unlock;
			}
		} else
			goto create_irqchip_unlock;
		r = kvm_setup_default_irq_routing(kvm);
		if (r) {
			mutex_lock(&kvm->slots_lock);
			mutex_lock(&kvm->irq_lock);
			kvm_ioapic_destroy(kvm);
			kvm_destroy_pic(vpic);
			mutex_unlock(&kvm->irq_lock);
			mutex_unlock(&kvm->slots_lock);
			goto create_irqchip_unlock;
		}
		 
		smp_wmb();
		kvm->arch.vpic = vpic;
	create_irqchip_unlock:
		mutex_unlock(&kvm->lock);
		break;
	}
	case KVM_CREATE_PIT:
		u.pit_config.flags = KVM_PIT_SPEAKER_DUMMY;
		goto create_pit;
	case KVM_CREATE_PIT2:
		r = -EFAULT;
		if (copy_from_user(&u.pit_config, argp,
				   sizeof(struct kvm_pit_config)))
			goto out;
	create_pit:
		mutex_lock(&kvm->slots_lock);
		r = -EEXIST;
		if (kvm->arch.vpit)
			goto create_pit_unlock;
		r = -ENOMEM;
		kvm->arch.vpit = kvm_create_pit(kvm, u.pit_config.flags);
		if (kvm->arch.vpit)
			r = 0;
	create_pit_unlock:
		mutex_unlock(&kvm->slots_lock);
		break;
	case KVM_GET_IRQCHIP: {
		 
		struct kvm_irqchip *chip;

		chip = memdup_user(argp, sizeof(*chip));
		if (IS_ERR(chip)) {
			r = PTR_ERR(chip);
			goto out;
		}

		r = -ENXIO;
		if (!irqchip_in_kernel(kvm) || irqchip_split(kvm))
			goto get_irqchip_out;
		r = kvm_vm_ioctl_get_irqchip(kvm, chip);
		if (r)
			goto get_irqchip_out;
		r = -EFAULT;
		if (copy_to_user(argp, chip, sizeof *chip))
			goto get_irqchip_out;
		r = 0;
	get_irqchip_out:
		kfree(chip);
		break;
	}
	case KVM_SET_IRQCHIP: {
		 
		struct kvm_irqchip *chip;

		chip = memdup_user(argp, sizeof(*chip));
		if (IS_ERR(chip)) {
			r = PTR_ERR(chip);
			goto out;
		}

		r = -ENXIO;
		if (!irqchip_in_kernel(kvm) || irqchip_split(kvm))
			goto set_irqchip_out;
		r = kvm_vm_ioctl_set_irqchip(kvm, chip);
		if (r)
			goto set_irqchip_out;
		r = 0;
	set_irqchip_out:
		kfree(chip);
		break;
	}
	case KVM_GET_PIT: {
		r = -EFAULT;
		if (copy_from_user(&u.ps, argp, sizeof(struct kvm_pit_state)))
			goto out;
		r = -ENXIO;
		if (!kvm->arch.vpit)
			goto out;
		r = kvm_vm_ioctl_get_pit(kvm, &u.ps);
		if (r)
			goto out;
		r = -EFAULT;
		if (copy_to_user(argp, &u.ps, sizeof(struct kvm_pit_state)))
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_PIT: {
		r = -EFAULT;
		if (copy_from_user(&u.ps, argp, sizeof u.ps))
			goto out;
		r = -ENXIO;
		if (!kvm->arch.vpit)
			goto out;
		r = kvm_vm_ioctl_set_pit(kvm, &u.ps);
		break;
	}
	case KVM_GET_PIT2: {
		r = -ENXIO;
		if (!kvm->arch.vpit)
			goto out;
		r = kvm_vm_ioctl_get_pit2(kvm, &u.ps2);
		if (r)
			goto out;
		r = -EFAULT;
		if (copy_to_user(argp, &u.ps2, sizeof(u.ps2)))
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_PIT2: {
		r = -EFAULT;
		if (copy_from_user(&u.ps2, argp, sizeof(u.ps2)))
			goto out;
		r = -ENXIO;
		if (!kvm->arch.vpit)
			goto out;
		r = kvm_vm_ioctl_set_pit2(kvm, &u.ps2);
		break;
	}
	case KVM_REINJECT_CONTROL: {
		struct kvm_reinject_control control;
		r =  -EFAULT;
		if (copy_from_user(&control, argp, sizeof(control)))
			goto out;
		r = kvm_vm_ioctl_reinject(kvm, &control);
		break;
	}
	case KVM_SET_BOOT_CPU_ID:
		r = 0;
		mutex_lock(&kvm->lock);
		if (atomic_read(&kvm->online_vcpus) != 0)
			r = -EBUSY;
		else
			kvm->arch.bsp_vcpu_id = arg;
		mutex_unlock(&kvm->lock);
		break;
	case KVM_XEN_HVM_CONFIG: {
		r = -EFAULT;
		if (copy_from_user(&kvm->arch.xen_hvm_config, argp,
				   sizeof(struct kvm_xen_hvm_config)))
			goto out;
		r = -EINVAL;
		if (kvm->arch.xen_hvm_config.flags)
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_CLOCK: {
		struct kvm_clock_data user_ns;
		u64 now_ns;
		s64 delta;

		r = -EFAULT;
		if (copy_from_user(&user_ns, argp, sizeof(user_ns)))
			goto out;

		r = -EINVAL;
		if (user_ns.flags)
			goto out;

		r = 0;
		local_irq_disable();
		now_ns = get_kernel_ns();
		delta = user_ns.clock - now_ns;
		local_irq_enable();
		kvm->arch.kvmclock_offset = delta;
		kvm_gen_update_masterclock(kvm);
		break;
	}
	case KVM_GET_CLOCK: {
		struct kvm_clock_data user_ns;
		u64 now_ns;

		local_irq_disable();
		now_ns = get_kernel_ns();
		user_ns.clock = kvm->arch.kvmclock_offset + now_ns;
		local_irq_enable();
		user_ns.flags = 0;
		memset(&user_ns.pad, 0, sizeof(user_ns.pad));

		r = -EFAULT;
		if (copy_to_user(argp, &user_ns, sizeof(user_ns)))
			goto out;
		r = 0;
		break;
	}
	case KVM_ENABLE_CAP: {
		struct kvm_enable_cap cap;

		r = -EFAULT;
		if (copy_from_user(&cap, argp, sizeof(cap)))
			goto out;
		r = kvm_vm_ioctl_enable_cap(kvm, &cap);
		break;
	}
	default:
		r = kvm_vm_ioctl_assigned_device(kvm, ioctl, arg);
	}
out:
	return r;
}