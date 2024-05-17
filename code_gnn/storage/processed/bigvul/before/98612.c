void ThumbnailGenerator::AskForThumbnail(RenderWidgetHost* renderer,
                                         ThumbnailReadyCallback* callback,
                                         gfx::Size size) {
  SkBitmap first_try = GetThumbnailForRenderer(renderer);
  if (!first_try.isNull()) {
    callback->Run(first_try);
    delete callback;
    return;
  }

  static int sequence_num = 0;
  TransportDIB* thumbnail_dib = TransportDIB::Create(
      size.width() * size.height() * 4, sequence_num++);
  linked_ptr<AsyncRequestInfo> request_info(new AsyncRequestInfo);
  request_info->callback.reset(callback);
  request_info->thumbnail_dib.reset(thumbnail_dib);
  request_info->renderer = renderer;
  ThumbnailCallbackMap::value_type new_value(thumbnail_dib->handle(),
                                             request_info);
  std::pair<ThumbnailCallbackMap::iterator, bool> result =
      callback_map_.insert(new_value);
  if (!result.second) {
    NOTREACHED() << "Callback already registered?";
    return;
  }

  renderer->PaintAtSize(thumbnail_dib->handle(), size);
}
