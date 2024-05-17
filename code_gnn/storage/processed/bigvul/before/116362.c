  void OnComplete(int result) {
    delete cache_;
    SetResult(result);
  }
