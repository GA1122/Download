static tBTA_AV_CODE bta_av_group_navi_supported(uint8_t len, uint8_t* p_data,
 bool is_inquiry) {
  tBTA_AV_CODE ret = AVRC_RSP_NOT_IMPL;
 uint8_t* p_ptr = p_data;
 uint16_t u16;
 uint32_t u32;

 if (p_bta_av_cfg->avrc_group && len == BTA_GROUP_NAVI_MSG_OP_DATA_LEN) {
    BTA_AV_BE_STREAM_TO_CO_ID(u32, p_ptr);
    BE_STREAM_TO_UINT16(u16, p_ptr);

 if (u32 == AVRC_CO_METADATA) {
 if (is_inquiry) {
 if (u16 <= AVRC_PDU_PREV_GROUP) ret = AVRC_RSP_IMPL_STBL;
 } else {
 if (u16 <= AVRC_PDU_PREV_GROUP)
          ret = AVRC_RSP_ACCEPT;
 else
          ret = AVRC_RSP_REJ;
 }
 }
 }

 return ret;
}
