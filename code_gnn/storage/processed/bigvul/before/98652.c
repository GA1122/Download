void RenderWidget::OnWasRestored(bool needs_repainting) {
  if (!webwidget_)
    return;

  SetHidden(false);

  if (!needs_repainting && !needs_repainting_on_restore_)
    return;
  needs_repainting_on_restore_ = false;

  set_next_paint_is_restore_ack();

  didInvalidateRect(gfx::Rect(size_.width(), size_.height()));
}
