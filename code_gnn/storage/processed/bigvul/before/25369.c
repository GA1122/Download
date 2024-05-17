static void mipspmu_disable(struct pmu *pmu)
{
	if (mipspmu)
		mipspmu->stop();
}
