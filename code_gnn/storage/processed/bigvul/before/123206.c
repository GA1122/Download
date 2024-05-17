void RenderWidgetHostViewAura::MovePluginWindows(
    const gfx::Vector2d& scroll_offset,
    const std::vector<webkit::npapi::WebPluginGeometry>& plugin_window_moves) {
#if defined(OS_WIN)
  if (!window_->GetRootWindow()) {
    DCHECK(plugin_window_moves.empty());
    return;
  }
  HWND parent = window_->GetRootWindow()->GetAcceleratedWidget();
  gfx::Rect view_bounds = window_->GetBoundsInRootWindow();
  std::vector<webkit::npapi::WebPluginGeometry> moves = plugin_window_moves;

  gfx::Rect view_port(scroll_offset.x(), scroll_offset.y(), view_bounds.width(),
                      view_bounds.height());

  for (size_t i = 0; i < moves.size(); ++i) {
    gfx::Rect clip(moves[i].clip_rect);
    gfx::Vector2d view_port_offset(
        moves[i].window_rect.OffsetFromOrigin() + scroll_offset);
    clip.Offset(view_port_offset);
    clip.Intersect(view_port);
    clip.Offset(-view_port_offset);
    moves[i].clip_rect = clip;

    moves[i].window_rect.Offset(view_bounds.OffsetFromOrigin());
  }
  MovePluginWindowsHelper(parent, moves);

  for (size_t i = 0; i < moves.size(); ++i) {
    HWND window = moves[i].window;
    if (GetParent(window) != parent) {
      window = GetParent(window);
      DCHECK(GetParent(window) == parent);
    }
    if (!GetProp(window, kWidgetOwnerProperty))
      CHECK(SetProp(window, kWidgetOwnerProperty, this));
  }
#endif   
}
