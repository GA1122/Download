void bta_av_rc_free_browse_msg(UNUSED_ATTR tBTA_AV_CB* p_cb,
                               tBTA_AV_DATA* p_data) {
 if (p_data->rc_msg.opcode == AVRC_OP_BROWSE) {
    osi_free_and_reset((void**)&p_data->rc_msg.msg.browse.p_browse_pkt);
 }
}
