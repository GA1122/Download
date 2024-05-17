void CastDetailedView::AppendSettingsEntries() {
  const bool userAddingRunning = Shell::GetInstance()
                                     ->session_state_delegate()
                                     ->IsInSecondaryLoginScreen();

  if (login_ == user::LOGGED_IN_NONE || login_ == user::LOGGED_IN_LOCKED ||
      userAddingRunning)
    return;

  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  HoverHighlightView* container = new HoverHighlightView(this);
  container->AddLabel(rb.GetLocalizedString(IDS_ASH_STATUS_TRAY_CAST_OPTIONS),
                      gfx::ALIGN_LEFT, false  );

  AddChildView(container);
  options_ = container;
}
