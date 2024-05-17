  virtual void Run() {
    NotificationService::current()->Notify(
        NotificationType::PRINT_JOB_EVENT,
        Source<PrintJob>(static_cast<PrintJob*>(print_job_.get())),
        Details<JobEventDetails>(details_));
  }
