void ScreenLayoutObserver::OnDisplayConfigurationChanged() {
  DisplayInfoMap old_info;
  UpdateDisplayInfo(&old_info);

  old_display_mode_ = current_display_mode_;
  if (GetDisplayManager()->IsInMirrorMode())
    current_display_mode_ = DisplayMode::MIRRORING;
  else if (GetDisplayManager()->IsInUnifiedMode())
    current_display_mode_ = DisplayMode::UNIFIED;
  else if (IsDockedModeEnabled())
    current_display_mode_ = DisplayMode::DOCKED;
  else if (GetDisplayManager()->GetNumDisplays() > 2)
    current_display_mode_ = DisplayMode::EXTENDED_3_PLUS;
  else if (GetDisplayManager()->GetNumDisplays() == 2)
    current_display_mode_ = DisplayMode::EXTENDED_2;
  else
    current_display_mode_ = DisplayMode::SINGLE;

  if (!show_notifications_for_testing)
    return;

  base::string16 message;
  base::string16 additional_message;
  if (GetDisplayMessageForNotification(old_info, &message, &additional_message))
    CreateOrUpdateNotification(message, additional_message);
}
