void RenderWidgetHostImpl::GetWebScreenInfo(WebKit::WebScreenInfo* result) {
#if defined(OS_POSIX) || defined(USE_AURA)
  if (GetView()) {
    static_cast<RenderWidgetHostViewPort*>(GetView())->GetScreenInfo(result);
  } else {
    RenderWidgetHostViewPort::GetDefaultScreenInfo(result);
  }
#else
  *result = WebKit::WebScreenInfoFactory::screenInfo(
      gfx::NativeViewFromId(GetNativeViewId()));
#endif
}
