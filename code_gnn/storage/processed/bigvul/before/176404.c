bool btif_av_stream_ready(void) {
 btif_sm_state_t state = btif_sm_get_state(btif_av_cb.sm_handle);

  BTIF_TRACE_DEBUG("%s: sm_handle=%d, state=%d, flags=0x%x", __func__,
                   btif_av_cb.sm_handle, state, btif_av_cb.flags);

  
 if (btif_is_enabled() == 0) {
    BTIF_TRACE_EVENT("%s: main adapter not enabled", __func__);
 return false;
 }

  
 if (btif_av_cb.flags &
 (BTIF_AV_FLAG_REMOTE_SUSPEND | BTIF_AV_FLAG_PENDING_STOP))
 return false;

 return (state == BTIF_AV_STATE_OPENED);
}
