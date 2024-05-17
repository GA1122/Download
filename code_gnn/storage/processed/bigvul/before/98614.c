WidgetThumbnail* GetDataForHost(RenderWidgetHost* host) {
  WidgetThumbnail* wt = GetThumbnailAccessor()->GetProperty(
      host->property_bag());
  if (wt)
    return wt;

  GetThumbnailAccessor()->SetProperty(host->property_bag(),
                                      WidgetThumbnail());
  return GetThumbnailAccessor()->GetProperty(host->property_bag());
}
