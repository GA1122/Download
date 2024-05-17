  void NotifyUpdatedActions() {
    media_controls_view_->MediaSessionActionsChanged(
        std::vector<MediaSessionAction>(actions_.begin(), actions_.end()));
  }
