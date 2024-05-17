static int create_server_sdp_record(rfc_slot_t *slot) {
 if(slot->scn == 0) {
 return false;
 }
  slot->sdp_handle = add_rfc_sdp_rec(slot->service_name, slot->service_uuid, slot->scn);
 return (slot->sdp_handle > 0);
}
