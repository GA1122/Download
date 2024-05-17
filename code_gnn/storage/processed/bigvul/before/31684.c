static inline bool intel_pmu_needs_lbr_smpl(struct perf_event *event)
{
	 
	if (has_branch_stack(event))
		return true;

	 
	if (x86_pmu.intel_cap.pebs_trap && event->attr.precise_ip > 1)
		return true;

	return false;
}
