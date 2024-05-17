pvscsi_ring_pop_cmp_descr(PVSCSIRingInfo *mgr)
{
     

    uint32_t free_cmp_ptr =
        mgr->filled_cmp_ptr++ & mgr->rxr_len_mask;
    uint32_t free_cmp_page =
        free_cmp_ptr / PVSCSI_MAX_NUM_CMP_ENTRIES_PER_PAGE;
    uint32_t inpage_idx =
        free_cmp_ptr % PVSCSI_MAX_NUM_CMP_ENTRIES_PER_PAGE;
    return mgr->cmp_ring_pages_pa[free_cmp_page] +
           inpage_idx * sizeof(PVSCSIRingCmpDesc);
}
