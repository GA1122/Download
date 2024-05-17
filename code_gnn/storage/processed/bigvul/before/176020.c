static tBTA_AV_CODE bta_av_op_supported(tBTA_AV_RC rc_id, bool is_inquiry) {
  tBTA_AV_CODE ret_code = AVRC_RSP_NOT_IMPL;

 if (p_bta_av_rc_id) {
 if (is_inquiry) {
 if (p_bta_av_rc_id[rc_id >> 4] & (1 << (rc_id & 0x0F))) {
        ret_code = AVRC_RSP_IMPL_STBL;
 }
 } else {
 if (p_bta_av_rc_id[rc_id >> 4] & (1 << (rc_id & 0x0F))) {
        ret_code = AVRC_RSP_ACCEPT;
 } else if ((p_bta_av_cfg->rc_pass_rsp == AVRC_RSP_INTERIM) &&
                 p_bta_av_rc_id_ac) {
 if (p_bta_av_rc_id_ac[rc_id >> 4] & (1 << (rc_id & 0x0F))) {
          ret_code = AVRC_RSP_INTERIM;
 }
 }
 }
 }
 return ret_code;
}
