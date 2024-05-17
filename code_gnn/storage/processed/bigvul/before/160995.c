void ChromeClientImpl::ScheduleAnimation(
    const PlatformFrameView* platform_frame_view) {
  DCHECK(platform_frame_view->IsLocalFrameView());
  LocalFrame& frame =
      ToLocalFrameView(platform_frame_view)->GetFrame().LocalFrameRoot();
  if (WebLocalFrameImpl::FromFrame(&frame) &&
      WebLocalFrameImpl::FromFrame(&frame)->FrameWidget())
    WebLocalFrameImpl::FromFrame(&frame)->FrameWidget()->ScheduleAnimation();
}
