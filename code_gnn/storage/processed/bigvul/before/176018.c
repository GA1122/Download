static uint8_t bta_av_get_shdl(tBTA_AV_SCB* p_scb) {
 int i;
 uint8_t shdl = 0;
  
 for (i = 0; i < BTA_AV_NUM_STRS; i++) {
 if (p_scb == bta_av_cb.p_scb[i]) {
      shdl = i + 1;
 break;
 }
 }
 return shdl;
}
