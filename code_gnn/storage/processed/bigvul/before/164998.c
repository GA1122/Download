CanvasRenderingContext* HTMLCanvasElement::GetCanvasRenderingContextInternal(
    const String& type,
    const CanvasContextCreationAttributesCore& attributes) {
  CanvasRenderingContext::ContextType context_type =
      CanvasRenderingContext::ContextTypeFromId(type);

  if (context_type == CanvasRenderingContext::kContextTypeUnknown ||
      (context_type == CanvasRenderingContext::kContextXRPresent &&
       !origin_trials::WebXREnabled(&GetDocument()))) {
    return nullptr;
  }

  if (!context_)
    UMA_HISTOGRAM_ENUMERATION("Blink.Canvas.ContextType", context_type);

  context_type =
      CanvasRenderingContext::ResolveContextTypeAliases(context_type);

  CanvasRenderingContextFactory* factory =
      GetRenderingContextFactory(context_type);
  if (!factory)
    return nullptr;

  if (context_) {
    if (context_->GetContextType() == context_type)
      return context_.Get();

    factory->OnError(this,
                     "Canvas has an existing context of a different type");
    return nullptr;
  }

  context_ = factory->Create(this, attributes);
  if (!context_)
    return nullptr;

  UMA_HISTOGRAM_BOOLEAN("Blink.Canvas.IsComposited", context_->IsComposited());

  context_creation_was_blocked_ = false;

  probe::didCreateCanvasContext(&GetDocument());

  if (Is3d())
    UpdateMemoryUsage();

  LayoutObject* layout_object = GetLayoutObject();
  if (layout_object && Is2d() && !context_->CreationAttributes().alpha) {
    DidDraw();
  }

  if (context_->CreationAttributes().low_latency &&
      origin_trials::LowLatencyCanvasEnabled(&GetDocument())) {
    CreateLayer();
    SetNeedsUnbufferedInputEvents(true);
    frame_dispatcher_ = std::make_unique<CanvasResourceDispatcher>(
        nullptr, surface_layer_bridge_->GetFrameSinkId().client_id(),
        surface_layer_bridge_->GetFrameSinkId().sink_id(),
        CanvasResourceDispatcher::kInvalidPlaceholderCanvasId, size_);
    frame_dispatcher_->SetNeedsBeginFrame(GetPage()->IsPageVisible());

    UseCounter::Count(GetDocument().GetFrame(),
                      WebFeature::kHTMLCanvasElementLowLatency);
  }

  SetNeedsCompositingUpdate();

  return context_.Get();
}
