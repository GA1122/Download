  void OnPresentation(const gfx::PresentationFeedback& feedback) {
    client_->DidReceivePresentationFeedback(feedback);
  }
