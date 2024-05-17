long kvm_arch_vcpu_ioctl(struct file *filp,
			 unsigned int ioctl, unsigned long arg)
{
	struct kvm_vcpu *vcpu = filp->private_data;
	void __user *argp = (void __user *)arg;
	struct kvm_ia64_vcpu_stack *stack = NULL;
	long r;

	switch (ioctl) {
	case KVM_IA64_VCPU_GET_STACK: {
		struct kvm_ia64_vcpu_stack __user *user_stack;
	        void __user *first_p = argp;

		r = -EFAULT;
		if (copy_from_user(&user_stack, first_p, sizeof(void *)))
			goto out;

		if (!access_ok(VERIFY_WRITE, user_stack,
			       sizeof(struct kvm_ia64_vcpu_stack))) {
			printk(KERN_INFO "KVM_IA64_VCPU_GET_STACK: "
			       "Illegal user destination address for stack\n");
			goto out;
		}
		stack = kzalloc(sizeof(struct kvm_ia64_vcpu_stack), GFP_KERNEL);
		if (!stack) {
			r = -ENOMEM;
			goto out;
		}

		r = kvm_arch_vcpu_ioctl_get_stack(vcpu, stack);
		if (r)
			goto out;

		if (copy_to_user(user_stack, stack,
				 sizeof(struct kvm_ia64_vcpu_stack))) {
			r = -EFAULT;
			goto out;
		}

		break;
	}
	case KVM_IA64_VCPU_SET_STACK: {
		struct kvm_ia64_vcpu_stack __user *user_stack;
	        void __user *first_p = argp;

		r = -EFAULT;
		if (copy_from_user(&user_stack, first_p, sizeof(void *)))
			goto out;

		if (!access_ok(VERIFY_READ, user_stack,
			    sizeof(struct kvm_ia64_vcpu_stack))) {
			printk(KERN_INFO "KVM_IA64_VCPU_SET_STACK: "
			       "Illegal user address for stack\n");
			goto out;
		}
		stack = kmalloc(sizeof(struct kvm_ia64_vcpu_stack), GFP_KERNEL);
		if (!stack) {
			r = -ENOMEM;
			goto out;
		}
		if (copy_from_user(stack, user_stack,
				   sizeof(struct kvm_ia64_vcpu_stack)))
			goto out;

		r = kvm_arch_vcpu_ioctl_set_stack(vcpu, stack);
		break;
	}

	default:
		r = -EINVAL;
	}

out:
	kfree(stack);
	return r;
}
