void RenderWidgetHostViewAura::OnActivated() {
  const ui::Event* event = window_->GetRootWindow()->current_event();
  if (event && PointerEventActivates(*event))
    host_->OnPointerEventActivate();
}
