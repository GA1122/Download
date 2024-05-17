static void p4_pmu_swap_config_ts(struct hw_perf_event *hwc, int cpu)
{
	u32 escr, cccr;

	 
	if (!p4_should_swap_ts(hwc->config, cpu))
		return;

	 

	escr = p4_config_unpack_escr(hwc->config);
	cccr = p4_config_unpack_cccr(hwc->config);

	if (p4_ht_thread(cpu)) {
		cccr &= ~P4_CCCR_OVF_PMI_T0;
		cccr |= P4_CCCR_OVF_PMI_T1;
		if (escr & P4_ESCR_T0_OS) {
			escr &= ~P4_ESCR_T0_OS;
			escr |= P4_ESCR_T1_OS;
		}
		if (escr & P4_ESCR_T0_USR) {
			escr &= ~P4_ESCR_T0_USR;
			escr |= P4_ESCR_T1_USR;
		}
		hwc->config  = p4_config_pack_escr(escr);
		hwc->config |= p4_config_pack_cccr(cccr);
		hwc->config |= P4_CONFIG_HT;
	} else {
		cccr &= ~P4_CCCR_OVF_PMI_T1;
		cccr |= P4_CCCR_OVF_PMI_T0;
		if (escr & P4_ESCR_T1_OS) {
			escr &= ~P4_ESCR_T1_OS;
			escr |= P4_ESCR_T0_OS;
		}
		if (escr & P4_ESCR_T1_USR) {
			escr &= ~P4_ESCR_T1_USR;
			escr |= P4_ESCR_T0_USR;
		}
		hwc->config  = p4_config_pack_escr(escr);
		hwc->config |= p4_config_pack_cccr(cccr);
		hwc->config &= ~P4_CONFIG_HT;
	}
}