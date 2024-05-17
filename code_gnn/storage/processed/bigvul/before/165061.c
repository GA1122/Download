CanvasRenderingContext* OffscreenCanvas::GetCanvasRenderingContext(
    ExecutionContext* execution_context,
    const String& id,
    const CanvasContextCreationAttributesCore& attributes) {
  execution_context_ = execution_context;

  CanvasRenderingContext::ContextType context_type =
      CanvasRenderingContext::ContextTypeFromId(id);

  if (context_type == CanvasRenderingContext::kContextTypeUnknown ||
      (context_type == CanvasRenderingContext::kContextXRPresent &&
       !origin_trials::WebXREnabled(execution_context))) {
    return nullptr;
  }

  if (!context_) {
    UMA_HISTOGRAM_ENUMERATION("Blink.OffscreenCanvas.ContextType",
                              context_type);
  }

  CanvasRenderingContextFactory* factory =
      GetRenderingContextFactory(context_type);
  if (!factory)
    return nullptr;

  if (context_) {
    if (context_->GetContextType() != context_type) {
      factory->OnError(
          this, "OffscreenCanvas has an existing context of a different type");
      return nullptr;
    }
  } else {
    context_ = factory->Create(this, attributes);
  }

  return context_.Get();
}
