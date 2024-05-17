void PrintJobWorker::OnNewPage() {
  if (!document_.get())   
    return;

  DCHECK(task_runner_->RunsTasksInCurrentSequence());

  if (page_number_ == PageNumber::npos()) {
    int page_count = document_->page_count();
    if (!page_count) {
      return;
    }
    page_number_.Init(document_->settings(), page_count);
  }
  DCHECK_NE(page_number_, PageNumber::npos());

  while (true) {
    scoped_refptr<PrintedPage> page = document_->GetPage(page_number_.ToInt());
    if (!page.get()) {
      base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
          FROM_HERE,
          base::BindOnce(&PrintJobWorker::OnNewPage,
                         weak_factory_.GetWeakPtr()),
          base::TimeDelta::FromMilliseconds(500));
      break;
    }
    SpoolPage(page.get());
    ++page_number_;
    if (page_number_ == PageNumber::npos()) {
      OnDocumentDone();
      break;
    }
  }
}
