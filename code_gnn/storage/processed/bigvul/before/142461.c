void ShelfLayoutManager::OnKeyboardAppearanceChanged(
    const keyboard::KeyboardStateDescriptor& state) {
  keyboard_occluded_bounds_ = state.occluded_bounds;
  keyboard_displaced_bounds_ = state.displaced_bounds;

  LayoutShelfAndUpdateBounds();
}
