static bool send_app_scn(rfc_slot_t *slot) {
 if(slot->scn_notified == true) {
 return true;
 }
  slot->scn_notified = true;
 return sock_send_all(slot->fd, (const uint8_t*)&slot->scn, sizeof(slot->scn)) == sizeof(slot->scn);
}
