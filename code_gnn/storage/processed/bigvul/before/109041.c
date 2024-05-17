bool RenderViewImpl::SupportsAsynchronousSwapBuffers() {
  if (WebWidgetHandlesCompositorScheduling() ||
      CommandLine::ForCurrentProcess()->HasSwitch(switches::kInProcessWebGL))
    return false;

  return true;
}
