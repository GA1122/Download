bool btif_av_is_peer_edr(void) {
  ASSERTC(btif_av_is_connected(), "No active a2dp connection", 0);

 if (btif_av_cb.edr)
 return true;
 else
 return false;
}
