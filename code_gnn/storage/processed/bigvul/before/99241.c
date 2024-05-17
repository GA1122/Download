void PrintJobWorker::OnDocumentChanged(PrintedDocument* new_document) {
  DCHECK_EQ(message_loop(), MessageLoop::current());
  DCHECK_EQ(page_number_, PageNumber::npos());
  DCHECK(!new_document ||
         new_document->settings().Equals(printing_context_.settings()));
#if !defined(OS_MACOSX)
  DCHECK(printing_context_.context());
#endif
  if (page_number_ != PageNumber::npos())
    return;

  document_ = new_document;
}
