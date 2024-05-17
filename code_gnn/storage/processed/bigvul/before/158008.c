void RenderViewImpl::ZoomLimitsChanged(double minimum_level,
                                       double maximum_level) {
  int minimum_percent = round(
      ZoomLevelToZoomFactor(minimum_level) * 100);
  int maximum_percent = round(
      ZoomLevelToZoomFactor(maximum_level) * 100);

  Send(new ViewHostMsg_UpdateZoomLimits(GetRoutingID(), minimum_percent,
                                        maximum_percent));
}
