void bta_av_rc_close(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
 uint16_t handle = p_data->hdr.layer_specific;
  tBTA_AV_SCB* p_scb;
  tBTA_AV_RCB* p_rcb;

 if (handle < BTA_AV_NUM_RCB) {
    p_rcb = &p_cb->rcb[handle];

    APPL_TRACE_DEBUG("%s: handle: %d, status=0x%x", __func__, p_rcb->handle,
                     p_rcb->status);
 if (p_rcb->handle != BTA_AV_RC_HANDLE_NONE) {
 if (p_rcb->shdl) {
        p_scb = bta_av_cb.p_scb[p_rcb->shdl - 1];
 if (p_scb) {
  
          alarm_cancel(p_scb->avrc_ct_timer);
 }
 }

      AVRC_Close(p_rcb->handle);
 }
 }
}
