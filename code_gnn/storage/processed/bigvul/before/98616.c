SkBitmap ThumbnailGenerator::GetThumbnailForRenderer(
    RenderWidgetHost* renderer) const {
  WidgetThumbnail* wt = GetDataForHost(renderer);

  BackingStore* backing_store = renderer->GetBackingStore(false);
  if (!backing_store) {
    return wt->thumbnail;
  }

  if (!wt->thumbnail.isNull() &&
      (no_timeout_ ||
       base::TimeTicks::Now() -
       base::TimeDelta::FromMilliseconds(kVisibilitySlopMS) < wt->last_shown))
    return wt->thumbnail;

  wt->thumbnail = GetThumbnailForBackingStore(backing_store);
  return wt->thumbnail;
}
