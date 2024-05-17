void TabStrip::PrepareForAnimation() {
  if (!drag_context_->IsDragSessionActive() &&
      !TabDragController::IsAttachedTo(GetDragContext())) {
    for (int i = 0; i < tab_count(); ++i)
      tab_at(i)->set_dragging(false);
  }
}
