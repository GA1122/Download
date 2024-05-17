void ShelfWidget::OnBackgroundTypeChanged(ShelfBackgroundType background_type,
                                          AnimationChangeType change_type) {
  delegate_view_->UpdateOpaqueBackground();
}
