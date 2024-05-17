void ThumbnailGenerator::WidgetShown(RenderWidgetHost* widget) {
  WidgetThumbnail* wt = GetDataForHost(widget);
  wt->last_shown = base::TimeTicks::Now();

  if (wt->thumbnail.isNull())
    return;

  std::vector<RenderWidgetHost*>::iterator found =
      std::find(shown_hosts_.begin(), shown_hosts_.end(), widget);
  if (found != shown_hosts_.end()) {
    NOTREACHED() << "Showing a RWH we already think is shown";
    shown_hosts_.erase(found);
  }
  shown_hosts_.push_back(widget);

  if (timer_.IsRunning())
    return;
  timer_.Start(base::TimeDelta::FromMilliseconds(
                   no_timeout_ ? 0 : kVisibilitySlopMS),
               this, &ThumbnailGenerator::ShownDelayHandler);
}
