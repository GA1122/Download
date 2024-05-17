LoginDisplayHostWebUI::~LoginDisplayHostWebUI() {
  DBusThreadManager::Get()->GetSessionManagerClient()->RemoveObserver(this);
  CrasAudioHandler::Get()->RemoveAudioObserver(this);
  display::Screen::GetScreen()->RemoveObserver(this);

  ui::InputDeviceManager::GetInstance()->RemoveObserver(this);

  if (login_view_ && login_window_)
    login_window_->RemoveRemovalsObserver(this);

  if (login_window_delegate_)
    login_window_delegate_->LoginDisplayHostDestroyed();

  MultiUserWindowManager* window_manager =
      MultiUserWindowManager::GetInstance();
  if (window_manager)
    window_manager->RemoveObserver(this);

  ResetKeyboardOverscrollOverride();

  views::FocusManager::set_arrow_key_traversal_enabled(false);
  ResetLoginWindowAndView();

  if (user_manager::UserManager::Get()->IsCurrentUserNew()) {
    (new DriveFirstRunController(ProfileManager::GetActiveUserProfile()))
        ->EnableOfflineMode();
  }
}
