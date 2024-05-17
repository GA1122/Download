HTMLCanvasElement::CreateAccelerated2dBuffer() {
  base::WeakPtr<WebGraphicsContext3DProviderWrapper> context_provider_wrapper =
      SharedGpuContext::ContextProviderWrapper();
  const bool needs_vertical_flip =
      !(context_provider_wrapper && context_provider_wrapper->ContextProvider()
                                        ->GetCapabilities()
                                        .mesa_framebuffer_flip_y);

  auto surface = std::make_unique<Canvas2DLayerBridge>(
      Size(), Canvas2DLayerBridge::kEnableAcceleration, ColorParams(),
      needs_vertical_flip);
  if (!surface->IsValid())
    return nullptr;

  if (MemoryPressureListenerRegistry::IsLowEndDevice())
    surface->DisableDeferral(kDisableDeferralReasonLowEndDevice);

  return surface;
}
