bt_status_t btsock_rfc_init(int poll_thread_handle) {
  pth = poll_thread_handle;

  memset(rfc_slots, 0, sizeof(rfc_slots));
 for (size_t i = 0; i < ARRAY_SIZE(rfc_slots); ++i) {
    rfc_slots[i].scn = -1;
    rfc_slots[i].sdp_handle = 0;
    rfc_slots[i].fd = INVALID_FD;
    rfc_slots[i].app_fd = INVALID_FD;
    rfc_slots[i].incoming_queue = list_new(GKI_freebuf);
    assert(rfc_slots[i].incoming_queue != NULL);
 }

  BTA_JvEnable(jv_dm_cback);

 return BT_STATUS_SUCCESS;
}
