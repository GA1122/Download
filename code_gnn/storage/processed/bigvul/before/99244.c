void PrintJobWorker::OnNewPage() {
  if (!document_.get()) {
    return;
  }
  DCHECK_EQ(message_loop(), MessageLoop::current());
#if !defined(OS_MACOSX)
  DCHECK(printing_context_.context());
  if (!printing_context_.context())
    return;
#endif

  if (page_number_ == PageNumber::npos()) {
    int page_count = document_->page_count();
    if (!page_count) {
      return;
    }
    page_number_.Init(document_->settings(), page_count);
  }
  DCHECK_NE(page_number_, PageNumber::npos());

  for (;;) {
    scoped_refptr<PrintedPage> page;
    if (!document_->GetPage(page_number_.ToInt(), &page)) {
      MessageLoop::current()->PostDelayedTask(
          FROM_HERE,
          NewRunnableMethod(this, &PrintJobWorker::OnNewPage),
          500);
      break;
    }
    SpoolPage(*page);
    ++page_number_;
    if (page_number_ == PageNumber::npos()) {
      OnDocumentDone();
      break;
    }
  }
}
