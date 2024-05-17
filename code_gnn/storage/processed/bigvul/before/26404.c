static int __init hw_break_module_init(void)
{
	int ret;
	struct perf_event_attr attr;

	hw_breakpoint_init(&attr);
	attr.bp_addr = kallsyms_lookup_name(ksym_name);
	attr.bp_len = HW_BREAKPOINT_LEN_4;
	attr.bp_type = HW_BREAKPOINT_W | HW_BREAKPOINT_R;

	sample_hbp = register_wide_hw_breakpoint(&attr, sample_hbp_handler);
	if (IS_ERR((void __force *)sample_hbp)) {
		ret = PTR_ERR((void __force *)sample_hbp);
		goto fail;
	}

	printk(KERN_INFO "HW Breakpoint for %s write installed\n", ksym_name);

	return 0;

fail:
	printk(KERN_INFO "Breakpoint registration failed\n");

	return ret;
}
