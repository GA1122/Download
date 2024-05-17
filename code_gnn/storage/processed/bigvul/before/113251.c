void ShelfLayoutManager::UpdateHitTestBounds() {
  gfx::Insets insets;
  if (state_.visibility_state == VISIBLE) {
    switch (alignment_) {
      case SHELF_ALIGNMENT_BOTTOM:
        insets.Set(kWorkspaceAreaBottomInset, 0, 0, 0);
        break;
      case SHELF_ALIGNMENT_LEFT:
        insets.Set(0, 0, 0, kWorkspaceAreaBottomInset);
        break;
      case SHELF_ALIGNMENT_RIGHT:
        insets.Set(0, kWorkspaceAreaBottomInset, 0, 0);
        break;
    }
  }
  if (launcher_widget() && launcher_widget()->GetNativeWindow())
    launcher_widget()->GetNativeWindow()->set_hit_test_bounds_override_outer(
        insets);
  status_->GetNativeWindow()->set_hit_test_bounds_override_outer(insets);
}
