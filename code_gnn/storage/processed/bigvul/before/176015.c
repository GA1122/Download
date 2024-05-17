tBTA_AV_LCB* bta_av_find_lcb(const RawAddress& addr, uint8_t op) {
  tBTA_AV_CB* p_cb = &bta_av_cb;
 int xx;
 uint8_t mask;
  tBTA_AV_LCB* p_lcb = NULL;

  APPL_TRACE_DEBUG("%s: address: %s op:%d", __func__, addr.ToString().c_str(),
                   op);
 for (xx = 0; xx < BTA_AV_NUM_LINKS; xx++) {
    mask = 1 << xx;  
 if ((mask & p_cb->conn_lcb) && p_cb->lcb[xx].addr == addr) {
      p_lcb = &p_cb->lcb[xx];
 if (op == BTA_AV_LCB_FREE) {
        p_cb->conn_lcb &= ~mask;  
        APPL_TRACE_DEBUG("%s: conn_lcb: 0x%x", __func__, p_cb->conn_lcb);
 }
 break;
 }
 }
 return p_lcb;
}
