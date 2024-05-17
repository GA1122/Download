void TabStrip::RemoveTabDelegate::HighlightCloseButton() {
  if (tabstrip_->IsDragSessionActive() ||
      !tabstrip_->ShouldHighlightCloseButtonAfterRemove()) {
    return;
  }

  views::Widget* widget = tabstrip_->GetWidget();
  if (!widget)
    return;

  widget->SynthesizeMouseMoveEvent();
}
