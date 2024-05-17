ResourceMessageFilter::~ResourceMessageFilter() {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  dom_storage_dispatcher_host_->Shutdown();

  db_dispatcher_host_->Shutdown();

  NotificationService::current()->Notify(
      NotificationType::RESOURCE_MESSAGE_FILTER_SHUTDOWN,
      Source<ResourceMessageFilter>(this),
      NotificationService::NoDetails());

  if (handle())
    base::CloseProcessHandle(handle());
}
