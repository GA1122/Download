  void SetEnabled(bool enabled) {
    controller_->SetEnabled(ImmersiveFullscreenController::WINDOW_TYPE_OTHER,
                            enabled);
  }
