void RenderViewImpl::didActivateCompositor(int input_handler_identifier) {
#if !defined(OS_MACOSX)  
#if !defined(OS_WIN)   
  CompositorThread* compositor_thread =
      RenderThreadImpl::current()->compositor_thread();
  if (compositor_thread)
    compositor_thread->AddInputHandler(
        routing_id_, input_handler_identifier, AsWeakPtr());
#endif
#endif

  RenderWidget::didActivateCompositor(input_handler_identifier);

  ProcessAcceleratedPinchZoomFlags(*CommandLine::ForCurrentProcess());
}
