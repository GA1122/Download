void omx_vdec::adjust_timestamp(OMX_S64 &act_timestamp)
{
 if (rst_prev_ts && VALID_TS(act_timestamp)) {
        prev_ts = act_timestamp;
        rst_prev_ts = false;
 } else if (VALID_TS(prev_ts)) {
 bool codec_cond = (drv_ctx.timestamp_adjust)?
 (!VALID_TS(act_timestamp) || act_timestamp < prev_ts || llabs(act_timestamp - prev_ts) <= 2000) :
 (!VALID_TS(act_timestamp) || act_timestamp <= prev_ts);
 if (frm_int > 0 && codec_cond) {
            DEBUG_PRINT_LOW("adjust_timestamp: original ts[%lld]", act_timestamp);
            act_timestamp = prev_ts + frm_int;
            DEBUG_PRINT_LOW("adjust_timestamp: predicted ts[%lld]", act_timestamp);
            prev_ts = act_timestamp;
 } else {
 if (drv_ctx.picture_order == VDEC_ORDER_DISPLAY && act_timestamp < prev_ts) {
                act_timestamp = prev_ts;
 }
            set_frame_rate(act_timestamp);
 }
 } else if (frm_int > 0)  
 {  
        act_timestamp = prev_ts = 0;  
        rst_prev_ts = true;
 }
}
