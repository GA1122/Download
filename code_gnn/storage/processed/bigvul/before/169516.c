  void Detach() {
    owner_ = nullptr;
    Remove();
  }
