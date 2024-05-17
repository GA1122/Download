static void bte_av_callback(tBTA_AV_EVT event, tBTA_AV* p_data) {
  btif_transfer_context(btif_av_handle_event, event, (char*)p_data,
 sizeof(tBTA_AV), btif_av_event_deep_copy);
}
