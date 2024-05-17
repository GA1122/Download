ProfileChooserView::ProfileChooserView(views::Button* anchor_button,
                                       Browser* browser,
                                       profiles::BubbleViewMode view_mode,
                                       signin::GAIAServiceType service_type,
                                       signin_metrics::AccessPoint access_point)
    : BubbleDialogDelegateView(anchor_button, views::BubbleBorder::TOP_RIGHT),
      browser_(browser),
      anchor_button_(anchor_button),
      view_mode_(view_mode),
      gaia_service_type_(service_type),
      access_point_(access_point),
      close_bubble_helper_(this, browser),
      dice_enabled_(AccountConsistencyModeManager::IsDiceEnabledForProfile(
          browser->profile())),
      menu_width_(dice_enabled_ ? kFixedMenuWidthDice
                                : kFixedMenuWidthPreDice) {
  set_margins(gfx::Insets(0, views::GridLayout::kFixedSize, 2, 0));
  ResetView();
  chrome::RecordDialogCreation(chrome::DialogIdentifier::PROFILE_CHOOSER);
}
