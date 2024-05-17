void ThumbnailGenerator::WidgetHidden(RenderWidgetHost* widget) {
  WidgetThumbnail* wt = GetDataForHost(widget);
  wt->last_hidden = base::TimeTicks::Now();

  EraseHostFromShownList(widget);

  if (!wt->thumbnail.isNull())
    return;
  wt->thumbnail = GetThumbnailForRenderer(widget);
}
