LockContentsView::LockContentsView(
    mojom::TrayActionState initial_note_action_state,
    LockScreen::ScreenType screen_type,
    LoginDataDispatcher* data_dispatcher,
    std::unique_ptr<LoginDetachableBaseModel> detachable_base_model)
    : NonAccessibleView(kLockContentsViewName),
      screen_type_(screen_type),
      data_dispatcher_(data_dispatcher),
      detachable_base_model_(std::move(detachable_base_model)),
      display_observer_(this),
      session_observer_(this) {
  data_dispatcher_->AddObserver(this);
  display_observer_.Add(display::Screen::GetScreen());
  Shell::Get()->login_screen_controller()->AddObserver(this);
  Shell::Get()->system_tray_notifier()->AddSystemTrayFocusObserver(this);
  keyboard::KeyboardController::Get()->AddObserver(this);
  auth_error_bubble_ = std::make_unique<LoginBubble>();
  detachable_base_error_bubble_ = std::make_unique<LoginBubble>();
  tooltip_bubble_ = std::make_unique<LoginBubble>();

  SetFocusBehavior(FocusBehavior::ALWAYS);

  SetLayoutManager(std::make_unique<views::FillLayout>());

  main_view_ = new NonAccessibleView();
  AddChildView(main_view_);

  top_header_ = new views::View();
  auto top_header_layout =
      std::make_unique<views::BoxLayout>(views::BoxLayout::kHorizontal);
  top_header_layout->set_main_axis_alignment(
      views::BoxLayout::MAIN_AXIS_ALIGNMENT_END);
  top_header_->SetLayoutManager(std::move(top_header_layout));
  AddChildView(top_header_);

  dev_channel_info_ = new views::View();
  auto dev_channel_info_layout = std::make_unique<views::BoxLayout>(
      views::BoxLayout::kVertical, gfx::Insets(5, 8));
  dev_channel_info_layout->set_cross_axis_alignment(
      views::BoxLayout::CROSS_AXIS_ALIGNMENT_END);
  dev_channel_info_->SetLayoutManager(std::move(dev_channel_info_layout));
  dev_channel_info_->SetVisible(false);
  top_header_->AddChildView(dev_channel_info_);

  note_action_ = new NoteActionLaunchButton(initial_note_action_state);
  top_header_->AddChildView(note_action_);

  expanded_view_ = new LoginExpandedPublicAccountView(
      base::BindRepeating(&LockContentsView::SetDisplayStyle,
                          base::Unretained(this), DisplayStyle::kAll));
  expanded_view_->SetVisible(false);
  AddChildView(expanded_view_);

  OnLockScreenNoteStateChanged(initial_note_action_state);
  chromeos::DBusThreadManager::Get()->GetPowerManagerClient()->AddObserver(
      this);
  RegisterAccelerators();
}
