uint64_t FrameIndexForView(RenderWidgetHostViewAura* view) {
  return ImageTransportFactory::GetInstance()
      ->GetContextFactoryPrivate()
      ->GetFrameSinkManager()
      ->surface_manager()
      ->GetSurfaceForId(view->GetCurrentSurfaceId())
      ->GetActiveFrameIndex();
}
