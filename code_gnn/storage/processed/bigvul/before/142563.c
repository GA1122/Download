void ShelfWidget::OnShelfAlignmentChanged() {
  CHECK(status_area_widget_);
  status_area_widget_->UpdateAfterShelfAlignmentChange();
  delegate_view_->UpdateOpaqueBackground();
}
