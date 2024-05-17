void RenderWidgetHostViewAura::CreateSelectionController() {
  ui::TouchSelectionController::Config tsc_config;
  tsc_config.max_tap_duration = base::TimeDelta::FromMilliseconds(
      ui::GestureConfiguration::GetInstance()->long_press_time_in_ms());
  tsc_config.tap_slop = ui::GestureConfiguration::GetInstance()
                            ->max_touch_move_in_pixels_for_click();
  tsc_config.enable_longpress_drag_selection = false;
  selection_controller_.reset(new ui::TouchSelectionController(
      selection_controller_client_.get(), tsc_config));
}
