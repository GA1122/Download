static void mipspmu_enable(struct pmu *pmu)
{
	if (mipspmu)
		mipspmu->start();
}
