  void OnClearClip(bool cleared) {
    OnClear(cleared);
    full_context_.clip_changed |= cleared;
  }
