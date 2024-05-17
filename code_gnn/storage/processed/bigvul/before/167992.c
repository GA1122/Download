LocalWindowProxy* LocalFrame::WindowProxy(DOMWrapperWorld& world) {
  return ToLocalWindowProxy(Frame::GetWindowProxy(world));
}
