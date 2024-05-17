int ShelfLayoutManager::GetWorkAreaInsets(const State& state, int size) const {
  if (IsKeyboardShown())
    return 0;
  if (state.visibility_state == SHELF_VISIBLE)
    return size;
  return 0;
}
