  void FlushHighlighterControllerMojo() {
    framework_service_->GetHighlighterClientForTesting()->FlushMojoForTesting();
  }
