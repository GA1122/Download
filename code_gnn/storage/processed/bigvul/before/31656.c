__intel_shared_reg_get_constraints(struct cpu_hw_events *cpuc,
				   struct perf_event *event,
				   struct hw_perf_event_extra *reg)
{
	struct event_constraint *c = &emptyconstraint;
	struct er_account *era;
	unsigned long flags;
	int idx = reg->idx;

	 
	if (reg->alloc && !cpuc->is_fake)
		return NULL;  

again:
	era = &cpuc->shared_regs->regs[idx];
	 
	raw_spin_lock_irqsave(&era->lock, flags);

	if (!atomic_read(&era->ref) || era->config == reg->config) {

		 
		if (!cpuc->is_fake) {
			if (idx != reg->idx)
				intel_fixup_er(event, idx);

			 
			reg->alloc = 1;
		}

		 
		era->config = reg->config;
		era->reg = reg->reg;

		 
		atomic_inc(&era->ref);

		 
		c = NULL;
	} else {
		idx = intel_alt_er(idx);
		if (idx != reg->idx) {
			raw_spin_unlock_irqrestore(&era->lock, flags);
			goto again;
		}
	}
	raw_spin_unlock_irqrestore(&era->lock, flags);

	return c;
}
