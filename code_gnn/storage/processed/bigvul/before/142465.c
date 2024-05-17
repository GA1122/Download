void ShelfLayoutManager::OnOverviewModeEndingAnimationComplete(bool canceled) {
  UpdateVisibilityState();
  MaybeUpdateShelfBackground(AnimationChangeType::ANIMATE);
}
