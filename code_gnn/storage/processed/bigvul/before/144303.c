void LoginDisplayHostWebUI::InitLoginWindowAndView() {
  if (login_window_)
    return;

  if (system::InputDeviceSettings::Get()->ForceKeyboardDrivenUINavigation()) {
    views::FocusManager::set_arrow_key_traversal_enabled(true);
    focus_ring_controller_ = std::make_unique<ash::FocusRingController>();
    focus_ring_controller_->SetVisible(true);

    keyboard_driven_oobe_key_handler_.reset(new KeyboardDrivenOobeKeyHandler);
  }

  views::Widget::InitParams params(
      views::Widget::InitParams::TYPE_WINDOW_FRAMELESS);
  params.bounds = CalculateScreenBounds(gfx::Size());
  if (!is_voice_interaction_oobe_)
    params.show_state = ui::SHOW_STATE_FULLSCREEN;
  params.opacity = views::Widget::InitParams::TRANSLUCENT_WINDOW;

  ash::ShellWindowId container = is_voice_interaction_oobe_
                                     ? ash::kShellWindowId_AlwaysOnTopContainer
                                     : ash::kShellWindowId_LockScreenContainer;
  if (features::IsAshInBrowserProcess()) {
    params.parent =
        ash::Shell::GetContainer(ash::Shell::GetPrimaryRootWindow(), container);
  } else {
    using ui::mojom::WindowManager;
    params.mus_properties[WindowManager::kContainerId_InitProperty] =
        mojo::ConvertTo<std::vector<uint8_t>>(static_cast<int32_t>(container));
  }
  login_window_ = new views::Widget;
  params.delegate = login_window_delegate_ =
      new LoginWidgetDelegate(login_window_, this);
  login_window_->Init(params);

  login_view_ = new WebUILoginView(WebUILoginView::WebViewSettings());
  login_view_->Init();
  if (login_view_->webui_visible())
    OnLoginPromptVisible();

  if (features::IsAshInBrowserProcess() && !is_voice_interaction_oobe_) {
    login_window_->SetVisibilityAnimationDuration(
        base::TimeDelta::FromMilliseconds(kLoginFadeoutTransitionDurationMs));
    login_window_->SetVisibilityAnimationTransition(
        views::Widget::ANIMATE_HIDE);
  }

  login_window_->AddRemovalsObserver(this);
  login_window_->SetContentsView(login_view_);

  if (!initialize_webui_hidden_ || !waiting_for_wallpaper_load_) {
    VLOG(1) << "Login WebUI >> show login wnd on create";
    login_window_->Show();
  } else {
    VLOG(1) << "Login WebUI >> login wnd is hidden on create";
    login_view_->set_is_hidden(true);
  }
  login_window_->GetNativeView()->SetName("WebUILoginView");
}
