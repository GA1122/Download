const gfx::Rect& DamageRectForView(RenderWidgetHostViewAura* view) {
  return ImageTransportFactory::GetInstance()
      ->GetContextFactoryPrivate()
      ->GetFrameSinkManager()
      ->surface_manager()
      ->GetSurfaceForId(view->GetCurrentSurfaceId())
      ->GetActiveFrame()
      .render_pass_list.back()
      ->damage_rect;
}
