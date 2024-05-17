void RenderWidgetHostViewAura::OnWindowActivated(aura::Window* gained_active,
                                                 aura::Window* lost_active) {
  DCHECK(window_ == gained_active || window_ == lost_active);
  if (window_ == gained_active) {
    const ui::Event* event = window_->GetDispatcher()->current_event();
    if (event && PointerEventActivates(*event))
      host_->OnPointerEventActivate();
  }
}
