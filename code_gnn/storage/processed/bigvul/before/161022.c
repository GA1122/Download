IntRect ChromeClientImpl::ViewportToScreen(
    const IntRect& rect_in_viewport,
    const PlatformFrameView* platform_frame_view) const {
  WebRect screen_rect(rect_in_viewport);

  DCHECK(platform_frame_view->IsLocalFrameView());
  const LocalFrameView* view = ToLocalFrameView(platform_frame_view);
  LocalFrame& frame = view->GetFrame().LocalFrameRoot();

  WebWidgetClient* client =
      WebLocalFrameImpl::FromFrame(&frame)->FrameWidget()->Client();

  if (client) {
    client->ConvertViewportToWindow(&screen_rect);
    WebRect view_rect = client->ViewRect();
    screen_rect.x += view_rect.x;
    screen_rect.y += view_rect.y;
  }

  return screen_rect;
}
