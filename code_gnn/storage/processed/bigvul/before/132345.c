    RenderFrameImpl::webScreenOrientationClient() {
  if (!screen_orientation_dispatcher_)
    screen_orientation_dispatcher_ = new ScreenOrientationDispatcher(this);
  return screen_orientation_dispatcher_;
}
