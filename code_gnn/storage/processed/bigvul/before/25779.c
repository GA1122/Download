static inline int x86_pmu_addr_offset(int index)
{
	int offset;

	 
	alternative_io(ASM_NOP2,
		       "shll $1, %%eax",
		       X86_FEATURE_PERFCTR_CORE,
		       "=a" (offset),
		       "a"  (index));

	return offset;
}