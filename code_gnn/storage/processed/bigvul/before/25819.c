static __init int intel_pmu_init(void)
{
	union cpuid10_edx edx;
	union cpuid10_eax eax;
	unsigned int unused;
	unsigned int ebx;
	int version;

	if (!cpu_has(&boot_cpu_data, X86_FEATURE_ARCH_PERFMON)) {
		switch (boot_cpu_data.x86) {
		case 0x6:
			return p6_pmu_init();
		case 0xf:
			return p4_pmu_init();
		}
		return -ENODEV;
	}

	 
	cpuid(10, &eax.full, &ebx, &unused, &edx.full);
	if (eax.split.mask_length <= ARCH_PERFMON_BRANCH_MISSES_RETIRED)
		return -ENODEV;

	version = eax.split.version_id;
	if (version < 2)
		x86_pmu = core_pmu;
	else
		x86_pmu = intel_pmu;

	x86_pmu.version			= version;
	x86_pmu.num_counters		= eax.split.num_counters;
	x86_pmu.cntval_bits		= eax.split.bit_width;
	x86_pmu.cntval_mask		= (1ULL << eax.split.bit_width) - 1;

	 
	if (version > 1)
		x86_pmu.num_counters_fixed = max((int)edx.split.num_counters_fixed, 3);

	 
	if (version > 1) {
		u64 capabilities;

		rdmsrl(MSR_IA32_PERF_CAPABILITIES, capabilities);
		x86_pmu.intel_cap.capabilities = capabilities;
	}

	intel_ds_init();

	 
	switch (boot_cpu_data.x86_model) {
	case 14:  
		pr_cont("Core events, ");
		break;

	case 15:  
		x86_pmu.quirks = intel_clovertown_quirks;
	case 22:  
	case 23:  
	case 29:  
		memcpy(hw_cache_event_ids, core2_hw_cache_event_ids,
		       sizeof(hw_cache_event_ids));

		intel_pmu_lbr_init_core();

		x86_pmu.event_constraints = intel_core2_event_constraints;
		x86_pmu.pebs_constraints = intel_core2_pebs_event_constraints;
		pr_cont("Core2 events, ");
		break;

	case 26:  
	case 30:  
	case 46:  
		memcpy(hw_cache_event_ids, nehalem_hw_cache_event_ids,
		       sizeof(hw_cache_event_ids));
		memcpy(hw_cache_extra_regs, nehalem_hw_cache_extra_regs,
		       sizeof(hw_cache_extra_regs));

		intel_pmu_lbr_init_nhm();

		x86_pmu.event_constraints = intel_nehalem_event_constraints;
		x86_pmu.pebs_constraints = intel_nehalem_pebs_event_constraints;
		x86_pmu.percore_constraints = intel_nehalem_percore_constraints;
		x86_pmu.enable_all = intel_pmu_nhm_enable_all;
		x86_pmu.extra_regs = intel_nehalem_extra_regs;

		 
		intel_perfmon_event_map[PERF_COUNT_HW_STALLED_CYCLES_FRONTEND] = 0x180010e;
		 
		intel_perfmon_event_map[PERF_COUNT_HW_STALLED_CYCLES_BACKEND] = 0x1803fb1;

		if (ebx & 0x40) {
			 
			intel_perfmon_event_map[PERF_COUNT_HW_BRANCH_MISSES] = 0x7f89;

			pr_cont("erratum AAJ80 worked around, ");
		}
		pr_cont("Nehalem events, ");
		break;

	case 28:  
		memcpy(hw_cache_event_ids, atom_hw_cache_event_ids,
		       sizeof(hw_cache_event_ids));

		intel_pmu_lbr_init_atom();

		x86_pmu.event_constraints = intel_gen_event_constraints;
		x86_pmu.pebs_constraints = intel_atom_pebs_event_constraints;
		pr_cont("Atom events, ");
		break;

	case 37:  
	case 44:  
	case 47:  
		memcpy(hw_cache_event_ids, westmere_hw_cache_event_ids,
		       sizeof(hw_cache_event_ids));
		memcpy(hw_cache_extra_regs, nehalem_hw_cache_extra_regs,
		       sizeof(hw_cache_extra_regs));

		intel_pmu_lbr_init_nhm();

		x86_pmu.event_constraints = intel_westmere_event_constraints;
		x86_pmu.percore_constraints = intel_westmere_percore_constraints;
		x86_pmu.enable_all = intel_pmu_nhm_enable_all;
		x86_pmu.pebs_constraints = intel_westmere_pebs_event_constraints;
		x86_pmu.extra_regs = intel_westmere_extra_regs;

		 
		intel_perfmon_event_map[PERF_COUNT_HW_STALLED_CYCLES_FRONTEND] = 0x180010e;
		 
		intel_perfmon_event_map[PERF_COUNT_HW_STALLED_CYCLES_BACKEND] = 0x1803fb1;

		pr_cont("Westmere events, ");
		break;

	case 42:  
		memcpy(hw_cache_event_ids, snb_hw_cache_event_ids,
		       sizeof(hw_cache_event_ids));

		intel_pmu_lbr_init_nhm();

		x86_pmu.event_constraints = intel_snb_event_constraints;
		x86_pmu.pebs_constraints = intel_snb_pebs_events;

		 
		intel_perfmon_event_map[PERF_COUNT_HW_STALLED_CYCLES_FRONTEND] = 0x180010e;
		 
		intel_perfmon_event_map[PERF_COUNT_HW_STALLED_CYCLES_BACKEND] = 0x18001b1;

		pr_cont("SandyBridge events, ");
		break;

	default:
		 
		x86_pmu.event_constraints = intel_gen_event_constraints;
		pr_cont("generic architected perfmon, ");
	}
	return 0;
}