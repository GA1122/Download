void XSyncHandler::PushPaintCounter(Display* display,
                                    Picture picture,
                                    Pixmap pixmap,
                                    const base::Closure& completion_callback) {
  backing_store_events_.push(
      new BackingStoreEvents(display, picture, pixmap, completion_callback));

  XSyncValue value;
  XSyncIntToValue(&value, 1);
  XSyncChangeCounter(ui::GetXDisplay(),
                     backing_store_sync_counter_,
                     value);
}
