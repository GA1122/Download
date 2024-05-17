void ShelfWidget::DelegateView::UpdateShelfBackground(SkColor color) {
  opaque_background_.SetColor(color);
  UpdateOpaqueBackground();
}
