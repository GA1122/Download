static void check_panic_on_oom(struct oom_control *oc,
			       enum oom_constraint constraint)
{
	if (likely(!sysctl_panic_on_oom))
		return;
	if (sysctl_panic_on_oom != 2) {
		 
		if (constraint != CONSTRAINT_NONE)
			return;
	}
	 
	if (is_sysrq_oom(oc))
		return;
	dump_header(oc, NULL);
	panic("Out of memory: %s panic_on_oom is enabled\n",
		sysctl_panic_on_oom == 2 ? "compulsory" : "system-wide");
}
