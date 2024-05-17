void Browser::CreateInstantIfNecessary() {
  if (type() == TYPE_NORMAL && InstantController::IsEnabled(profile()) &&
      !profile()->IsOffTheRecord()) {
    instant_.reset(new InstantController(profile_, this));
    instant_unload_handler_.reset(new InstantUnloadHandler(this));
  }
}
