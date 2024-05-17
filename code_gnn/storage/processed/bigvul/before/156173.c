RendererSchedulerImpl::CompositorThreadOnly::CompositorThreadOnly()
    : last_input_type(blink::WebInputEvent::kUndefined),
      main_thread_seems_unresponsive(false) {}
