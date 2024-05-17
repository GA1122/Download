bool InputHandlerProxy::ShouldAnimate(bool has_precise_scroll_deltas) const {
#if defined(OS_MACOSX)
  return false;
#else
  return smooth_scroll_enabled_ && !has_precise_scroll_deltas;
#endif
}
