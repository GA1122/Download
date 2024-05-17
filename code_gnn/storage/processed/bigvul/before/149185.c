void LockScreenMediaControlsView::UpdateActionButtonsVisibility() {
  std::set<MediaSessionAction> ignored_actions = {
      media_message_center::GetPlayPauseIgnoredAction(
          media_message_center::GetActionFromButtonTag(*play_pause_button_))};

  std::set<MediaSessionAction> visible_actions =
      media_message_center::GetTopVisibleActions(enabled_actions_,
                                                 ignored_actions, kMaxActions);

  for (auto* view : button_row_->children()) {
    views::Button* action_button = views::Button::AsButton(view);
    bool should_show = base::Contains(
        visible_actions,
        media_message_center::GetActionFromButtonTag(*action_button));

    action_button->SetVisible(should_show);
  }

  PreferredSizeChanged();
}
