static int init_hyp_mode(void)
{
	int cpu;
	int err = 0;

	 
	err = kvm_mmu_init();
	if (err)
		goto out_err;

	 
	hyp_default_vectors = __hyp_get_vectors();

	 
	for_each_possible_cpu(cpu) {
		unsigned long stack_page;

		stack_page = __get_free_page(GFP_KERNEL);
		if (!stack_page) {
			err = -ENOMEM;
			goto out_free_stack_pages;
		}

		per_cpu(kvm_arm_hyp_stack_page, cpu) = stack_page;
	}

	 
	err = create_hyp_mappings(__kvm_hyp_code_start, __kvm_hyp_code_end);
	if (err) {
		kvm_err("Cannot map world-switch code\n");
		goto out_free_mappings;
	}

	 
	for_each_possible_cpu(cpu) {
		char *stack_page = (char *)per_cpu(kvm_arm_hyp_stack_page, cpu);
		err = create_hyp_mappings(stack_page, stack_page + PAGE_SIZE);

		if (err) {
			kvm_err("Cannot map hyp stack\n");
			goto out_free_mappings;
		}
	}

	 
	kvm_host_cpu_state = alloc_percpu(kvm_cpu_context_t);
	if (!kvm_host_cpu_state) {
		err = -ENOMEM;
		kvm_err("Cannot allocate host CPU state\n");
		goto out_free_mappings;
	}

	for_each_possible_cpu(cpu) {
		kvm_cpu_context_t *cpu_ctxt;

		cpu_ctxt = per_cpu_ptr(kvm_host_cpu_state, cpu);
		err = create_hyp_mappings(cpu_ctxt, cpu_ctxt + 1);

		if (err) {
			kvm_err("Cannot map host CPU state: %d\n", err);
			goto out_free_context;
		}
	}

	 
	on_each_cpu(cpu_init_hyp_mode, NULL, 1);

	 
	err = kvm_vgic_hyp_init();
	if (err)
		goto out_free_context;

#ifdef CONFIG_KVM_ARM_VGIC
		vgic_present = true;
#endif

	 
	err = kvm_timer_hyp_init();
	if (err)
		goto out_free_mappings;

#ifndef CONFIG_HOTPLUG_CPU
	free_boot_hyp_pgd();
#endif

	kvm_perf_init();

	kvm_info("Hyp mode initialized successfully\n");

	return 0;
out_free_context:
	free_percpu(kvm_host_cpu_state);
out_free_mappings:
	free_hyp_pgds();
out_free_stack_pages:
	for_each_possible_cpu(cpu)
		free_page(per_cpu(kvm_arm_hyp_stack_page, cpu));
out_err:
	kvm_err("error initializing Hyp mode: %d\n", err);
	return err;
}
