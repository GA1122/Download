  void Init(PrintJobWorkerOwner* print_job,
            JobEventDetails::Type detail_type,
            PrintedDocument* document,
            PrintedPage* page) {
    DCHECK(!print_job_);
    DCHECK(!details_);
    print_job_ = print_job;
    details_ = new JobEventDetails(detail_type, document, page);
  }
