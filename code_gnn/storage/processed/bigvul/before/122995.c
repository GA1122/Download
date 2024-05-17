void RenderWidgetHostImpl::NotifyScreenInfoChanged() {
  WebKit::WebScreenInfo screen_info;
  GetWebScreenInfo(&screen_info);
  Send(new ViewMsg_ScreenInfoChanged(GetRoutingID(), screen_info));
}
