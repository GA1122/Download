void RenderWidget::OnResize(const gfx::Size& new_size,
                            const gfx::Rect& resizer_rect) {
  if (!webwidget_)
    return;

  resizer_rect_ = resizer_rect;

  SetHidden(false);
  needs_repainting_on_restore_ = false;

  DCHECK(size_ != new_size);
  size_ = new_size;

  DCHECK(!next_paint_is_resize_ack());

  paint_aggregator_.ClearPendingUpdate();

  webwidget_->resize(new_size);
  if (!new_size.IsEmpty()) {
    DCHECK(paint_aggregator_.HasPendingUpdate());

    set_next_paint_is_resize_ack();
  }
}
