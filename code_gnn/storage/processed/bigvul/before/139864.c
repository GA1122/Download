Shell::~Shell() {
  TRACE_EVENT0("shutdown", "ash::Shell::Destructor");

  user_metrics_recorder_->OnShellShuttingDown();

  delegate_->PreShutdown();

  views::FocusManagerFactory::Install(nullptr);

  aura::client::GetFocusClient(GetPrimaryRootWindow())->FocusWindow(nullptr);

  if (window_modality_controller_)
    window_modality_controller_.reset();
  RemovePreTargetHandler(
      window_tree_host_manager_->input_method_event_handler());
#if defined(OS_CHROMEOS)
  RemovePreTargetHandler(magnifier_key_scroll_handler_.get());
  magnifier_key_scroll_handler_.reset();

  RemovePreTargetHandler(speech_feedback_handler_.get());
  speech_feedback_handler_.reset();
#endif
  RemovePreTargetHandler(overlay_filter_.get());
  RemovePreTargetHandler(accelerator_filter_.get());
  RemovePreTargetHandler(event_transformation_handler_.get());
  RemovePreTargetHandler(toplevel_window_event_handler_.get());
  RemovePostTargetHandler(toplevel_window_event_handler_.get());
  RemovePreTargetHandler(system_gesture_filter_.get());
  RemovePreTargetHandler(keyboard_metrics_filter_.get());
  RemovePreTargetHandler(mouse_cursor_filter_.get());

  RemovePreTargetHandler(tooltip_controller_.get());

#if defined(OS_CHROMEOS)
  screen_orientation_controller_.reset();
#endif

#if defined(OS_CHROMEOS)
  virtual_keyboard_controller_.reset();
#endif

  maximize_mode_controller_.reset();

  app_list_controller_.reset();

#if defined(OS_CHROMEOS)
  last_window_closed_logout_reminder_.reset();

  logout_confirmation_controller_.reset();
#endif

  DeactivateKeyboard();

  HideShelf();
  system_tray_delegate_->Shutdown();
  system_tray_delegate_.reset();

  locale_notification_controller_.reset();

  drag_drop_controller_.reset();


#if defined(OS_CHROMEOS)
  video_activity_notifier_.reset();
#endif   
  video_detector_.reset();
  high_contrast_controller_.reset();

  shadow_controller_.reset();
  resize_shadow_controller_.reset();

  window_cycle_controller_.reset();
  window_selector_controller_.reset();

  shelf_window_watcher_.reset();

  window_tree_host_manager_->CloseChildWindows();
  mru_window_tracker_.reset();

  shelf_delegate_.reset();
  focus_client_.reset();

  system_tray_notifier_.reset();

  magnification_controller_.reset();
  partial_magnification_controller_.reset();
  tooltip_controller_.reset();
  event_client_.reset();
  toplevel_window_event_handler_.reset();
  visibility_controller_.reset();
  shelf_item_delegate_manager_.reset();
  shelf_model_.reset();

  power_button_controller_.reset();
  lock_state_controller_.reset();

#if defined(OS_CHROMEOS)
  resolution_notification_controller_.reset();
#endif
  desktop_background_controller_.reset();
  partial_screenshot_controller_.reset();
  mouse_cursor_filter_.reset();

#if defined(OS_CHROMEOS)
  touch_transformer_controller_.reset();
#endif   

  display_manager_->CreateScreenForShutdown();
  display_configuration_controller_.reset();
  window_tree_host_manager_->Shutdown();
  window_tree_host_manager_.reset();
  screen_position_controller_.reset();
  accessibility_delegate_.reset();
  new_window_delegate_.reset();
  media_delegate_.reset();

  keyboard::KeyboardController::ResetInstance(nullptr);

#if defined(OS_CHROMEOS)
  display_color_manager_.reset();
  if (display_change_observer_)
    display_configurator_->RemoveObserver(display_change_observer_.get());
  if (display_error_observer_)
    display_configurator_->RemoveObserver(display_error_observer_.get());
  if (projecting_observer_) {
    display_configurator_->RemoveObserver(projecting_observer_.get());
    RemoveShellObserver(projecting_observer_.get());
  }
  display_change_observer_.reset();

  PowerStatus::Shutdown();

  DCHECK(chromeos::DBusThreadManager::IsInitialized());
#endif

  DCHECK(instance_ == this);
  instance_ = nullptr;
}
