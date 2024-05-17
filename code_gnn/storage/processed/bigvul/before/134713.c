  void Destroy() {
    if (destroyed_)
      return;

    destroyed_ = true;
    guest_->EmbedderWillBeDestroyed();
    guest_->Destroy();
  }
