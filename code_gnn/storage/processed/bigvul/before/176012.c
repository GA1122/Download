static void bta_av_del_sdp_rec(uint32_t* p_sdp_handle) {
 if (*p_sdp_handle != 0) {
    SDP_DeleteRecord(*p_sdp_handle);
 *p_sdp_handle = 0;
 }
}
