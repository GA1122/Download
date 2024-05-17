static void mmtimer_setup_int_0(int cpu, u64 expires)
{
	u64 val;

	 
	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_RTC1_INT_ENABLE), 0UL);

	 
	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_INT_CMPB), -1L);

	 
	mmtimer_clr_int_pending(0);

	val = ((u64)SGI_MMTIMER_VECTOR << SH_RTC1_INT_CONFIG_IDX_SHFT) |
		((u64)cpu_physical_id(cpu) <<
			SH_RTC1_INT_CONFIG_PID_SHFT);

	 
	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_RTC1_INT_CONFIG), val);

	 
	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_RTC1_INT_ENABLE), 1UL);

	 
	HUB_S((u64 *)LOCAL_MMR_ADDR(SH_INT_CMPB), expires);


}
