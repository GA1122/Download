void TabStrip::RemoveTabDelegate::AnimationEnded(
    const gfx::Animation* animation) {
  DCHECK(tab()->closing());
  tab_strip()->RemoveAndDeleteTab(tab());

  if (!tab_strip()->GetDragContext()->IsDragSessionActive() &&
      tab_strip()->ShouldHighlightCloseButtonAfterRemove()) {
    views::Widget* widget = tab_strip()->GetWidget();
    if (widget)
      widget->SynthesizeMouseMoveEvent();
  }
}
