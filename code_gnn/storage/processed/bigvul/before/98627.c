void ThumbnailGenerator::WidgetWillDestroyBackingStore(
    RenderWidgetHost* widget,
    BackingStore* backing_store) {
  WidgetThumbnail* wt = GetDataForHost(widget);

  if (!wt->thumbnail.isNull())
    return;

  SkBitmap new_thumbnail = GetThumbnailForBackingStore(backing_store);
  if (!new_thumbnail.isNull())
    wt->thumbnail = new_thumbnail;
}
