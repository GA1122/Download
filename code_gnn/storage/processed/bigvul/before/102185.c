void SyncManager::SyncInternal::SetJsEventHandler(
    const WeakHandle<JsEventHandler>& event_handler) {
  js_event_handler_ = event_handler;
  js_sync_manager_observer_.SetJsEventHandler(js_event_handler_);
  js_transaction_observer_.SetJsEventHandler(js_event_handler_);
}
