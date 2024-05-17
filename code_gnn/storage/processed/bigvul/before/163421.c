RendererBlinkPlatformImpl* RenderThreadImpl::current_blink_platform_impl() {
  if (g_current_blink_platform_impl_for_testing)
    return g_current_blink_platform_impl_for_testing;
  DCHECK(current());
  return current()->blink_platform_impl();
}
