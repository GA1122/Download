void ProfileSyncService::OnUnrecoverableErrorImpl(
    const tracked_objects::Location& from_here,
    const std::string& message,
    bool delete_sync_database) {
  unrecoverable_error_detected_ = true;
  unrecoverable_error_message_ = message;
  unrecoverable_error_location_ = from_here;

  NotifyObservers();
  std::string location;
  from_here.Write(true, true, &location);
  LOG(ERROR)
      << "Unrecoverable error detected at " << location
      << " -- ProfileSyncService unusable: " << message;

  MessageLoop::current()->PostTask(FROM_HERE,
      base::Bind(&ProfileSyncService::ShutdownImpl, weak_factory_.GetWeakPtr(),
                 delete_sync_database));
}
