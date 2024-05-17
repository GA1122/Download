void RenderFrameImpl::RenderWidgetWillHandleMouseEvent() {
#if BUILDFLAG(ENABLE_PLUGINS)
  pepper_last_mouse_event_target_ = nullptr;
#endif
}
