static void evmcs_load(u64 phys_addr)
{
	struct hv_vp_assist_page *vp_ap =
		hv_get_vp_assist_page(smp_processor_id());

	vp_ap->current_nested_vmcs = phys_addr;
	vp_ap->enlighten_vmentry = 1;
}
