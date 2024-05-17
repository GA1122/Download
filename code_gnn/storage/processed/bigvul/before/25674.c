static bool __init supported_pmu(void)
{
	if (!strcmp(sparc_pmu_type, "ultra3") ||
	    !strcmp(sparc_pmu_type, "ultra3+") ||
	    !strcmp(sparc_pmu_type, "ultra3i") ||
	    !strcmp(sparc_pmu_type, "ultra4+")) {
		sparc_pmu = &ultra3_pmu;
		return true;
	}
	if (!strcmp(sparc_pmu_type, "niagara")) {
		sparc_pmu = &niagara1_pmu;
		return true;
	}
	if (!strcmp(sparc_pmu_type, "niagara2")) {
		sparc_pmu = &niagara2_pmu;
		return true;
	}
	return false;
}
