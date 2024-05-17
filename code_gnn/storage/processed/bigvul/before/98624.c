void ThumbnailGenerator::WidgetDidUpdateBackingStore(
    RenderWidgetHost* widget) {
  WidgetThumbnail* wt = GetThumbnailAccessor()->GetProperty(
      widget->property_bag());
  if (!wt)
    return;   

  if (no_timeout_ ||
      base::TimeTicks::Now() -
      base::TimeDelta::FromMilliseconds(kVisibilitySlopMS) < wt->last_shown)
    return;   

  wt->thumbnail = SkBitmap();
}
