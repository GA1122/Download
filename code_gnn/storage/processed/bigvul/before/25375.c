static int mipspmu_get_irq(void)
{
	int err;

	if (mipspmu->irq >= 0) {
		 
		err = request_irq(mipspmu->irq, mipspmu->handle_irq,
			IRQF_DISABLED | IRQF_NOBALANCING,
			"mips_perf_pmu", NULL);
		if (err) {
			pr_warning("Unable to request IRQ%d for MIPS "
			   "performance counters!\n", mipspmu->irq);
		}
	} else if (cp0_perfcount_irq < 0) {
		 
		save_perf_irq = perf_irq;
		perf_irq = mipspmu->handle_shared_irq;
		err = 0;
	} else {
		pr_warning("The platform hasn't properly defined its "
			"interrupt controller.\n");
		err = -ENOENT;
	}

	return err;
}
