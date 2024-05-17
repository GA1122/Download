void ShelfLayoutManager::StopAnimating() {
  GetLayer(shelf_widget_)->GetAnimator()->StopAnimating();
  GetLayer(shelf_widget_->status_area_widget())->GetAnimator()->StopAnimating();
}
