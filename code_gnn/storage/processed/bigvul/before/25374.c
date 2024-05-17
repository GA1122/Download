static void mipspmu_free_irq(void)
{
	if (mipspmu->irq >= 0)
		free_irq(mipspmu->irq, NULL);
	else if (cp0_perfcount_irq < 0)
		perf_irq = save_perf_irq;
}
