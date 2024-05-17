void NotificationCallback(PrintJobWorkerOwner* print_job,
                          JobEventDetails::Type detail_type,
                          int job_id,
                          PrintedDocument* document,
                          PrintedPage* page) {
  JobEventDetails* details =
      new JobEventDetails(detail_type, job_id, document, page);
  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_PRINT_JOB_EVENT,
      content::Source<PrintJob>(static_cast<PrintJob*>(print_job)),
      content::Details<JobEventDetails>(details));
}
