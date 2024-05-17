PrintingContext::Result PrintingContext::NewDocument(
    const std::wstring& document_name) {
  DCHECK(!in_print_job_);

  NOTIMPLEMENTED();

#ifndef NDEBUG
  page_number_ = 0;
#endif

  return FAILED;
}
