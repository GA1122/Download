intel_percore_constraints(struct cpu_hw_events *cpuc, struct perf_event *event)
{
	struct hw_perf_event *hwc = &event->hw;
	unsigned int e = hwc->config & ARCH_PERFMON_EVENTSEL_EVENT;
	struct event_constraint *c;
	struct intel_percore *pc;
	struct er_account *era;
	int i;
	int free_slot;
	int found;

	if (!x86_pmu.percore_constraints || hwc->extra_alloc)
		return NULL;

	for (c = x86_pmu.percore_constraints; c->cmask; c++) {
		if (e != c->code)
			continue;

		 
		pc = cpuc->per_core;
		if (!pc)
			break;
		c = &emptyconstraint;
		raw_spin_lock(&pc->lock);
		free_slot = -1;
		found = 0;
		for (i = 0; i < MAX_EXTRA_REGS; i++) {
			era = &pc->regs[i];
			if (era->ref > 0 && hwc->extra_reg == era->extra_reg) {
				 
				if (hwc->extra_config == era->extra_config) {
					era->ref++;
					cpuc->percore_used = 1;
					hwc->extra_alloc = 1;
					c = NULL;
				}
				 
				found = 1;
				break;
			} else if (era->ref == 0 && free_slot == -1)
				free_slot = i;
		}
		if (!found && free_slot != -1) {
			era = &pc->regs[free_slot];
			era->ref = 1;
			era->extra_reg = hwc->extra_reg;
			era->extra_config = hwc->extra_config;
			cpuc->percore_used = 1;
			hwc->extra_alloc = 1;
			c = NULL;
		}
		raw_spin_unlock(&pc->lock);
		return c;
	}

	return NULL;
}