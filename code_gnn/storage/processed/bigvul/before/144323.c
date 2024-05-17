void LoginDisplayHostWebUI::OnStartUserAdding() {
  DisableKeyboardOverscroll();

  restore_path_ = RESTORE_ADD_USER_INTO_SESSION;
  if (features::IsAshInBrowserProcess())
    finalize_animation_type_ = ANIMATION_ADD_USER;
  MultiUserWindowManager* window_manager =
      MultiUserWindowManager::GetInstance();
  if (window_manager)
    window_manager->AddObserver(this);

  VLOG(1) << "Login WebUI >> user adding";
  if (!login_window_)
    LoadURL(GURL(kUserAddingURL));
  login_view_->set_should_emit_login_prompt_visible(false);

  if (features::IsAshInBrowserProcess()) {
    aura::Window* lock_container = ash::Shell::GetContainer(
        ash::Shell::GetPrimaryRootWindow(),
        ash::kShellWindowId_LockScreenContainersContainer);
    lock_container->layer()->SetOpacity(1.0);
  } else {
    NOTIMPLEMENTED();
  }

  CreateExistingUserController();

  if (!signin_screen_controller_.get()) {
    signin_screen_controller_.reset(new SignInScreenController(GetOobeUI()));
  }

  SetOobeProgressBarVisible(oobe_progress_bar_visible_ = false);
  SetStatusAreaVisible(true);
  existing_user_controller_->Init(
      user_manager::UserManager::Get()->GetUsersAllowedForMultiProfile());
  CHECK(login_display_);
  GetOobeUI()->ShowSigninScreen(LoginScreenContext(), login_display_.get(),
                                login_display_.get());
}
