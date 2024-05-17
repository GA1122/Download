void RenderWidgetHostViewAura::OnBoundsChanged(const gfx::Rect& old_bounds,
                                               const gfx::Rect& new_bounds) {
  base::AutoReset<bool> in_bounds_changed(&in_bounds_changed_, true);
  SetSize(new_bounds.size());

  if (GetInputMethod())
    GetInputMethod()->OnCaretBoundsChanged(this);
}
