int ShelfLayoutManager::GetWorkAreaSize(const State& state, int size) const {
  if (state.visibility_state == VISIBLE)
    return size;
  if (state.visibility_state == AUTO_HIDE)
    return kAutoHideSize;
  return 0;
}
