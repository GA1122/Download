  pvscsi_ring_pop_req_descr(PVSCSIRingInfo *mgr)
  {
      uint32_t ready_ptr = RS_GET_FIELD(mgr, reqProdIdx);
//     uint32_t ring_size = PVSCSI_MAX_NUM_PAGES_REQ_RING
//                             * PVSCSI_MAX_NUM_REQ_ENTRIES_PER_PAGE;
  
    if (ready_ptr != mgr->consumed_ptr) {
//     if (ready_ptr != mgr->consumed_ptr
//         && ready_ptr - mgr->consumed_ptr < ring_size) {
          uint32_t next_ready_ptr =
              mgr->consumed_ptr++ & mgr->txr_len_mask;
          uint32_t next_ready_page =
 
         return mgr->req_ring_pages_pa[next_ready_page] +
                inpage_idx * sizeof(PVSCSIRingReqDesc);
     } else {
         return 0;
     }
 }