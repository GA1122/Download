static inline void evmcs_touch_msr_bitmap(void)
{
	if (unlikely(!current_evmcs))
		return;

	if (current_evmcs->hv_enlightenments_control.msr_bitmap)
		current_evmcs->hv_clean_fields &=
			~HV_VMX_ENLIGHTENED_CLEAN_FIELD_MSR_BITMAP;
}
