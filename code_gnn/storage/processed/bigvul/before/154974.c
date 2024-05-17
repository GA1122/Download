void WebGLRenderingContextBase::ProvideBackBufferToResourceProvider() const {
  if (isContextLost())
    return;

  DCHECK(Host()->ResourceProvider());
  if (Host()->ResourceProvider()->Size() != GetDrawingBuffer()->Size())
    Host()->DiscardResourceProvider();

  CanvasResourceProvider* resource_provider =
      Host()->GetOrCreateCanvasResourceProvider(kPreferAcceleration);
  if (!resource_provider || !resource_provider->IsAccelerated())
    return;

  resource_provider->ImportResource(
      GetDrawingBuffer()->AsCanvasResource(resource_provider->CreateWeakPtr()));
}
