static int __init init_hw_perf_events(void)
{
	struct event_constraint *c;
	int err;

	pr_info("Performance Events: ");

	switch (boot_cpu_data.x86_vendor) {
	case X86_VENDOR_INTEL:
		err = intel_pmu_init();
		break;
	case X86_VENDOR_AMD:
		err = amd_pmu_init();
		break;
	default:
		return 0;
	}
	if (err != 0) {
		pr_cont("no PMU driver, software events only.\n");
		return 0;
	}

	pmu_check_apic();

	 
	if (!check_hw_exists())
		return 0;

	pr_cont("%s PMU driver.\n", x86_pmu.name);

	if (x86_pmu.quirks)
		x86_pmu.quirks();

	if (x86_pmu.num_counters > X86_PMC_MAX_GENERIC) {
		WARN(1, KERN_ERR "hw perf events %d > max(%d), clipping!",
		     x86_pmu.num_counters, X86_PMC_MAX_GENERIC);
		x86_pmu.num_counters = X86_PMC_MAX_GENERIC;
	}
	x86_pmu.intel_ctrl = (1 << x86_pmu.num_counters) - 1;

	if (x86_pmu.num_counters_fixed > X86_PMC_MAX_FIXED) {
		WARN(1, KERN_ERR "hw perf events fixed %d > max(%d), clipping!",
		     x86_pmu.num_counters_fixed, X86_PMC_MAX_FIXED);
		x86_pmu.num_counters_fixed = X86_PMC_MAX_FIXED;
	}

	x86_pmu.intel_ctrl |=
		((1LL << x86_pmu.num_counters_fixed)-1) << X86_PMC_IDX_FIXED;

	perf_events_lapic_init();
	register_die_notifier(&perf_event_nmi_notifier);

	unconstrained = (struct event_constraint)
		__EVENT_CONSTRAINT(0, (1ULL << x86_pmu.num_counters) - 1,
				   0, x86_pmu.num_counters);

	if (x86_pmu.event_constraints) {
		for_each_event_constraint(c, x86_pmu.event_constraints) {
			if (c->cmask != X86_RAW_EVENT_MASK)
				continue;

			c->idxmsk64 |= (1ULL << x86_pmu.num_counters) - 1;
			c->weight += x86_pmu.num_counters;
		}
	}

	pr_info("... version:                %d\n",     x86_pmu.version);
	pr_info("... bit width:              %d\n",     x86_pmu.cntval_bits);
	pr_info("... generic registers:      %d\n",     x86_pmu.num_counters);
	pr_info("... value mask:             %016Lx\n", x86_pmu.cntval_mask);
	pr_info("... max period:             %016Lx\n", x86_pmu.max_period);
	pr_info("... fixed-purpose events:   %d\n",     x86_pmu.num_counters_fixed);
	pr_info("... event mask:             %016Lx\n", x86_pmu.intel_ctrl);

	perf_pmu_register(&pmu, "cpu", PERF_TYPE_RAW);
	perf_cpu_notifier(x86_pmu_notifier);

	return 0;
}