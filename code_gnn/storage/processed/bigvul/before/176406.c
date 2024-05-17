void btif_dispatch_sm_event(btif_av_sm_event_t event, void* p_data, int len) {
  
  btif_transfer_context(btif_av_handle_event, event, (char*)p_data, len, NULL);
}
