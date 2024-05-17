bool btif_av_stream_started_ready(void) {
 btif_sm_state_t state = btif_sm_get_state(btif_av_cb.sm_handle);
 bool ready = false;

  
 if (btif_av_cb.flags &
 (BTIF_AV_FLAG_LOCAL_SUSPEND_PENDING | BTIF_AV_FLAG_REMOTE_SUSPEND |
       BTIF_AV_FLAG_PENDING_STOP)) {
    ready = false;
 } else {
    ready = (state == BTIF_AV_STATE_STARTED);
 }

  BTIF_TRACE_WARNING("%s: sm_handle=%d state=%d flags=0x%x ready=%d", __func__,
                     btif_av_cb.sm_handle, state, btif_av_cb.flags, ready);

 return ready;
}
