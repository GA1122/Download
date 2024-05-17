void SyncManager::SyncInternal::OnNotificationStateChange(
    bool notifications_enabled) {
  DVLOG(1) << "P2P: Notifications enabled = "
           << (notifications_enabled ? "true" : "false");
  allstatus_.SetNotificationsEnabled(notifications_enabled);
  if (scheduler()) {
    scheduler()->set_notifications_enabled(notifications_enabled);
  }
  if (js_event_handler_.IsInitialized()) {
    DictionaryValue details;
    details.Set("enabled", Value::CreateBooleanValue(notifications_enabled));
    js_event_handler_.Call(FROM_HERE,
                           &JsEventHandler::HandleJsEvent,
                           "onNotificationStateChange",
                           JsEventDetails(&details));
  }
}
