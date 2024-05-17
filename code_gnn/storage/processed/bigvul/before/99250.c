void PrintJobWorker::StartPrinting(PrintedDocument* new_document) {
  DCHECK_EQ(message_loop(), MessageLoop::current());
  DCHECK_EQ(page_number_, PageNumber::npos());
  DCHECK_EQ(document_, new_document);
  DCHECK(document_.get());
  DCHECK(new_document->settings().Equals(printing_context_.settings()));
#if !defined(OS_MACOSX)
  DCHECK(printing_context_.context());
#endif
  if (!document_.get() || page_number_ != PageNumber::npos() ||
      document_ != new_document) {
    return;
  }

  PrintingContext::Result result =
      printing_context_.NewDocument(document_->name());
  if (result != PrintingContext::OK) {
    OnFailure();
    return;
  }

  OnNewPage();
}
