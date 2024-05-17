void ProfileSyncService::ClearUnrecoverableError() {
  unrecoverable_error_detected_ = false;
  unrecoverable_error_message_.clear();
  unrecoverable_error_location_ = tracked_objects::Location();
}
