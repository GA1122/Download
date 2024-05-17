static void session_set_fx_enabled(struct session_s *session, uint32_t id, bool enabled)
{
 if (enabled) {
 if(session->enabled_msk == 0) {
  
 }
        session->enabled_msk |= (1 << id);
 } else {
        session->enabled_msk &= ~(1 << id);
 if(session->enabled_msk == 0) {
  
 }
 }
    ALOGV("session_set_fx_enabled() id %d, enabled %d enabled_msk %08x",
         id, enabled, session->enabled_msk);
    session->processed_msk = 0;
}
