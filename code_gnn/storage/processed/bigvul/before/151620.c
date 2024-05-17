void ResourceLoader::ScheduleCancel() {
  if (!cancel_timer_.IsActive())
    cancel_timer_.StartOneShot(0, BLINK_FROM_HERE);
}
