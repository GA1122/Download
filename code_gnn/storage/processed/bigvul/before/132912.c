void RenderWidgetHostViewAura::BuffersSwapped(
    const gfx::Size& surface_size,
    const gfx::Rect& damage_rect,
    float surface_scale_factor,
    const std::string& mailbox_name,
    const std::vector<ui::LatencyInfo>& latency_info,
    const BufferPresentedCallback& ack_callback) {
  scoped_refptr<ui::Texture> previous_texture(current_surface_);
  const gfx::Rect surface_rect = gfx::Rect(surface_size);
  software_frame_manager_->DiscardCurrentFrame();

  if (!SwapBuffersPrepare(surface_rect,
                          surface_scale_factor,
                          damage_rect,
                          mailbox_name,
                          ack_callback)) {
    return;
  }

  SkRegion damage(RectToSkIRect(damage_rect));
  if (!skipped_damage_.isEmpty()) {
    damage.op(skipped_damage_, SkRegion::kUnion_Op);
    skipped_damage_.setEmpty();
  }

  DCHECK(surface_rect.Contains(SkIRectToRect(damage.getBounds())));
  ui::Texture* current_texture = current_surface_.get();

  const gfx::Size surface_size_in_pixel = surface_size;
  DLOG_IF(ERROR, previous_texture.get() &&
      previous_texture->size() != current_texture->size() &&
      SkIRectToRect(damage.getBounds()) != surface_rect) <<
      "Expected full damage rect after size change";
  if (previous_texture.get() && !previous_damage_.isEmpty() &&
      previous_texture->size() == current_texture->size()) {
    ImageTransportFactory* factory = ImageTransportFactory::GetInstance();
    GLHelper* gl_helper = factory->GetGLHelper();
    gl_helper->CopySubBufferDamage(
        current_texture->PrepareTexture(),
        previous_texture->PrepareTexture(),
        damage,
        previous_damage_);
  }
  previous_damage_ = damage;

  ui::Compositor* compositor = GetCompositor();
  if (compositor) {
    gfx::Rect rect_to_paint =
        ConvertRectToDIP(surface_scale_factor,
                         gfx::Rect(damage_rect.x(),
                                   surface_size_in_pixel.height() -
                                       damage_rect.y() - damage_rect.height(),
                                   damage_rect.width(),
                                   damage_rect.height()));

    rect_to_paint.Inset(-1, -1);
    rect_to_paint.Intersect(window_->bounds());

    if (paint_observer_)
      paint_observer_->OnUpdateCompositorContent();
    window_->SchedulePaintInRect(rect_to_paint);
    for (size_t i = 0; i < latency_info.size(); i++)
      compositor->SetLatencyInfo(latency_info[i]);
  }

  SwapBuffersCompleted(ack_callback, previous_texture);
}
