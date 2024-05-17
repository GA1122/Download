void mca_ccb_report_event(tMCA_CCB* p_ccb, uint8_t event, tMCA_CTRL* p_data) {
 if (p_ccb && p_ccb->p_rcb && p_ccb->p_rcb->p_cback)
 (*p_ccb->p_rcb->p_cback)(mca_rcb_to_handle(p_ccb->p_rcb),
                             mca_ccb_to_hdl(p_ccb), event, p_data);
}
