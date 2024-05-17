void mca_ccb_rsp_tout(tMCA_CCB* p_ccb, UNUSED_ATTR tMCA_CCB_EVT* p_data) {
  tMCA_CTRL evt_data;

  mca_ccb_report_event(p_ccb, MCA_RSP_TOUT_IND_EVT, &evt_data);
}
