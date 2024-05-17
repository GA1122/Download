static int enter_vmx_operation(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	struct vmcs *shadow_vmcs;
	int r;

	r = alloc_loaded_vmcs(&vmx->nested.vmcs02);
	if (r < 0)
		goto out_vmcs02;

	vmx->nested.cached_vmcs12 = kmalloc(VMCS12_SIZE, GFP_KERNEL);
	if (!vmx->nested.cached_vmcs12)
		goto out_cached_vmcs12;

	if (enable_shadow_vmcs) {
		shadow_vmcs = alloc_vmcs();
		if (!shadow_vmcs)
			goto out_shadow_vmcs;
		 
		shadow_vmcs->revision_id |= (1u << 31);
		 
		vmcs_clear(shadow_vmcs);
		vmx->vmcs01.shadow_vmcs = shadow_vmcs;
	}

	hrtimer_init(&vmx->nested.preemption_timer, CLOCK_MONOTONIC,
		     HRTIMER_MODE_REL_PINNED);
	vmx->nested.preemption_timer.function = vmx_preemption_timer_fn;

	vmx->nested.vmxon = true;
	return 0;

out_shadow_vmcs:
	kfree(vmx->nested.cached_vmcs12);

out_cached_vmcs12:
	free_loaded_vmcs(&vmx->nested.vmcs02);

out_vmcs02:
	return -ENOMEM;
}