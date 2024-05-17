void mca_ccb_dl_open(tMCA_CCB* p_ccb, UNUSED_ATTR tMCA_CCB_EVT* p_data) {
  osi_free_and_reset((void**)&p_ccb->p_tx_req);
  osi_free_and_reset((void**)&p_ccb->p_rx_msg);
  p_ccb->status = MCA_CCB_STAT_NORM;
}
