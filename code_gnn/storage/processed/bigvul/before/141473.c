bool PaintLayerScrollableArea::ScheduleAnimation() {
  if (ChromeClient* client = GetChromeClient()) {
    client->ScheduleAnimation(GetLayoutBox()->GetFrame()->View());
    return true;
  }
  return false;
}
