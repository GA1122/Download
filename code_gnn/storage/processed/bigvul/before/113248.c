void ShelfLayoutManager::StopAnimating() {
  if (launcher_widget())
    GetLayer(launcher_widget())->GetAnimator()->StopAnimating();
  GetLayer(status_)->GetAnimator()->StopAnimating();
}
