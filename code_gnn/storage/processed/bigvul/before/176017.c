tBTA_AV_RCB* bta_av_get_rcb_by_shdl(uint8_t shdl) {
  tBTA_AV_RCB* p_rcb = NULL;
 int i;

 for (i = 0; i < BTA_AV_NUM_RCB; i++) {
 if (bta_av_cb.rcb[i].shdl == shdl &&
        bta_av_cb.rcb[i].handle != BTA_AV_RC_HANDLE_NONE) {
      p_rcb = &bta_av_cb.rcb[i];
 break;
 }
 }
 return p_rcb;
}
