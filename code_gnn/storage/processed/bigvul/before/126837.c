void BrowserView::PaintChildren(gfx::Canvas* canvas) {
  for (int i = 0; i < child_count(); ++i) {
    View* child = child_at(i);
    if (child != infobar_container_)
      child->Paint(canvas);
  }

  infobar_container_->Paint(canvas);
}
