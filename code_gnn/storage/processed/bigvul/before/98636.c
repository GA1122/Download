void RenderWidget::GenerateFullRepaint() {
  didInvalidateRect(gfx::Rect(size_.width(), size_.height()));
}
