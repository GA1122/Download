static uint8_t bta_av_find_lcb_index_by_scb_and_address(
 const RawAddress& peer_address) {
  APPL_TRACE_DEBUG("%s: peer_address: %s conn_lcb: 0x%x", __func__,
                   peer_address.ToString().c_str(), bta_av_cb.conn_lcb);

 for (uint8_t index = 0; index < BTA_AV_NUM_LINKS; index++) {
 uint8_t mask = 1 << index;
 if (mask & bta_av_cb.conn_lcb) {
 continue;
 }
    tBTA_AV_SCB* p_scb = bta_av_cb.p_scb[index];
 if (p_scb == nullptr) {
 continue;
 }
 if (p_scb->PeerAddress() == peer_address) {
 return index;
 }
 }

 for (uint8_t index = 0; index < BTA_AV_NUM_LINKS; index++) {
 uint8_t mask = 1 << index;
 if (mask & bta_av_cb.conn_lcb) {
 continue;
 }
    tBTA_AV_SCB* p_scb = bta_av_cb.p_scb[index];
 if (p_scb == nullptr) {
 continue;
 }
 if (!p_scb->IsAssigned()) {
 return index;
 }
 }

 return BTA_AV_NUM_LINKS;
}
