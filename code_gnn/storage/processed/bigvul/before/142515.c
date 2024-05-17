  void NotifyKeyboardChanging(ShelfLayoutManager* layout_manager,
                              bool is_locked,
                              const gfx::Rect& bounds) {
    keyboard::KeyboardStateDescriptor state;
    state.visual_bounds = bounds;
    state.occluded_bounds = bounds;
    state.displaced_bounds = is_locked ? bounds : gfx::Rect();
    state.is_visible = !bounds.IsEmpty();
    layout_manager->OnKeyboardVisibilityStateChanged(state.is_visible);
    layout_manager->OnKeyboardAppearanceChanged(state);
  }
