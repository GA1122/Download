  bool IsLoadingSelection() {
    return owns_web_view_ && frame() && frame()->isLoading();
  }
