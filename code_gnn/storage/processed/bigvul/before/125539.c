GDataDirectoryService::~GDataDirectoryService() {
  ClearRoot();

  if (blocking_task_runner_ && directory_service_db_.get())
    blocking_task_runner_->DeleteSoon(FROM_HERE,
                                       directory_service_db_.release());
 }
