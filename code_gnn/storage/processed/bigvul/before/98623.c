void ThumbnailGenerator::WidgetDidReceivePaintAtSizeAck(
    RenderWidgetHost* widget,
    const TransportDIB::Handle& dib_handle,
    const gfx::Size& size) {
  ThumbnailCallbackMap::iterator item = callback_map_.find(dib_handle);
  if (item != callback_map_.end()) {
    TransportDIB* dib = item->second->thumbnail_dib.get();
    DCHECK(dib);
    if (!dib) {
      return;
    }

    SkBitmap non_owned_bitmap;
    SkBitmap result;

    non_owned_bitmap.setConfig(SkBitmap::kARGB_8888_Config,
                               size.width(), size.height());
    non_owned_bitmap.setPixels(dib->memory());

    non_owned_bitmap.copyTo(&result, SkBitmap::kARGB_8888_Config);

    item->second->callback->Run(result);

    callback_map_.erase(item);
  }
}
