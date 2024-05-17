void RenderThreadImpl::SetRendererBlinkPlatformImplForTesting(
    RendererBlinkPlatformImpl* blink_platform_impl) {
  g_current_blink_platform_impl_for_testing = blink_platform_impl;
}
