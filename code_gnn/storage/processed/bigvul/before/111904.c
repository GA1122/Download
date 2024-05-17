void ProfileSyncService::NotifyObservers() {
  FOR_EACH_OBSERVER(Observer, observers_, OnStateChanged());
  sync_js_controller_.HandleJsEvent(
      "onServiceStateChanged", JsEventDetails());
}
