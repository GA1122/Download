bool ShelfLayoutManager::IsKeyboardShown() const {
  return !keyboard_displaced_bounds_.IsEmpty();
}
