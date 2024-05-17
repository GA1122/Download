void Browser::CreateInstantIfNecessary() {
  if (is_type_tabbed() && InstantController::IsEnabled(profile()) &&
      !profile()->IsOffTheRecord()) {
    instant_.reset(new InstantController(profile_, this));
    instant_unload_handler_.reset(new InstantUnloadHandler(this));
  }
}
