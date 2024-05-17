void Layer::CreateRenderSurface() {
  DCHECK(!draw_properties_.render_surface);
  draw_properties_.render_surface = make_scoped_ptr(new RenderSurface(this));
  draw_properties_.render_target = this;
}
