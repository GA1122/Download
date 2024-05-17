void ChromeClientImpl::AutoscrollFling(WebFloatSize velocity,
                                       LocalFrame* local_frame) {
  LocalFrame& local_root = local_frame->LocalFrameRoot();
  if (WebFrameWidgetBase* widget =
          WebLocalFrameImpl::FromFrame(&local_root)->FrameWidget())
    widget->Client()->AutoscrollFling(velocity);
}
