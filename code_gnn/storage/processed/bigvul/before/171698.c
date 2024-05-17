static void bt_jni_msg_ready(void *context) {
  BT_HDR *p_msg = (BT_HDR *)context;

  BTIF_TRACE_VERBOSE("btif task fetched event %x", p_msg->event);

 switch (p_msg->event) {
 case BT_EVT_CONTEXT_SWITCH_EVT:
      btif_context_switched(p_msg);
 break;
 default:
      BTIF_TRACE_ERROR("unhandled btif event (%d)", p_msg->event & BT_EVT_MASK);
 break;
 }
  GKI_freebuf(p_msg);
}
