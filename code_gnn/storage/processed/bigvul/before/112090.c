void SyncManager::SyncInternal::NotifyCryptographerState(
    Cryptographer * cryptographer) {
  allstatus_.SetCryptographerReady(cryptographer->is_ready());
  allstatus_.SetCryptoHasPendingKeys(cryptographer->has_pending_keys());
  debug_info_event_listener_.SetCryptographerReady(cryptographer->is_ready());
  debug_info_event_listener_.SetCrytographerHasPendingKeys(
      cryptographer->has_pending_keys());
}
