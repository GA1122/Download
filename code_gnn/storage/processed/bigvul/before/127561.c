XSyncHandler::~XSyncHandler() {
  if (loaded_extension_)
    gdk_window_remove_filter(NULL, &OnEventThunk, this);

  while (!backing_store_events_.empty()) {
    BackingStoreEvents* data = backing_store_events_.front();
    backing_store_events_.pop();
    XRenderFreePicture(data->display, data->picture);
    XFreePixmap(data->display, data->pixmap);
    delete data;
  }
}
