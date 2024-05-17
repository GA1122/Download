void avrc_process_timeout(void* data) {
  tAVRC_PARAM* param = (tAVRC_PARAM*)data;

  AVRC_TRACE_DEBUG("AVRC: command timeout (handle=0x%02x, label=0x%02x)",
                   param->handle, param->label);

  
 if (avrc_cb.ccb[param->handle].ctrl_cback) {
    avrc_cb.ccb[param->handle].ctrl_cback.Run(
        param->handle, AVRC_CMD_TIMEOUT_EVT, param->label, NULL);
 }

  
 if (param->msg_mask & AVRC_MSG_MASK_IS_VENDOR_CMD) {
    avrc_send_next_vendor_cmd(param->handle);
 }
  osi_free(param);
}
