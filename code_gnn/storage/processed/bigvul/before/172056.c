static void free_rfc_slot_scn(rfc_slot_t *slot) {
 if (slot->scn <= 0)
 return;

 if(slot->f.server && !slot->f.closing && slot->rfc_handle) {
    BTA_JvRfcommStopServer(slot->rfc_handle, (void *)(uintptr_t)slot->id);
    slot->rfc_handle = 0;
 }

 if (slot->f.server)
    BTM_FreeSCN(slot->scn);
  slot->scn = 0;
}
