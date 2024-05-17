bool RenderViewImpl::SupportsAsynchronousSwapBuffers() {
  if (RenderThreadImpl::current()->compositor_message_loop_proxy() ||
      CommandLine::ForCurrentProcess()->HasSwitch(switches::kInProcessWebGL))
    return false;

  return true;
}
