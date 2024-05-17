void RenderWidgetHostViewAura::SetMainFrameAXTreeID(
    ui::AXTreeIDRegistry::AXTreeID id) {
  window_->SetProperty(ui::kChildAXTreeID, id);
}
