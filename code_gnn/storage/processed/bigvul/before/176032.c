void bta_av_rc_meta_rsp(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  tBTA_AV_RCB* p_rcb;
 bool do_free = true;

 if ((p_cb->features & BTA_AV_FEAT_METADATA) &&
 (p_data->hdr.layer_specific < BTA_AV_NUM_RCB)) {
 if ((p_data->api_meta_rsp.is_rsp && (p_cb->features & BTA_AV_FEAT_RCTG)) ||
 (!p_data->api_meta_rsp.is_rsp && (p_cb->features & BTA_AV_FEAT_RCCT))) {
      p_rcb = &p_cb->rcb[p_data->hdr.layer_specific];
 if (p_rcb->handle != BTA_AV_RC_HANDLE_NONE) {
        AVRC_MsgReq(p_rcb->handle, p_data->api_meta_rsp.label,
                    p_data->api_meta_rsp.rsp_code, p_data->api_meta_rsp.p_pkt);
        do_free = false;
 }
 }
 }

 if (do_free) osi_free_and_reset((void**)&p_data->api_meta_rsp.p_pkt);
}
