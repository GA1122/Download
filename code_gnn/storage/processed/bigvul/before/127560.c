XSyncHandler::XSyncHandler()
    : loaded_extension_(false),
      xsync_event_base_(0),
      xsync_error_base_(0),
      backing_store_sync_counter_(0),
      backing_store_sync_alarm_(0) {
  Display* display = ui::GetXDisplay();
  if (XSyncQueryExtension(display,
                          &xsync_event_base_,
                          &xsync_error_base_)) {
    XSyncValue value;
    XSyncIntToValue(&value, 0);
    backing_store_sync_counter_ = XSyncCreateCounter(display, value);

    XSyncAlarmAttributes attributes;
    attributes.trigger.counter = backing_store_sync_counter_;
    backing_store_sync_alarm_ = XSyncCreateAlarm(display,
                                                 XSyncCACounter,
                                                 &attributes);

    gdk_window_add_filter(NULL, &OnEventThunk, this);

    loaded_extension_ = true;
  }
}
