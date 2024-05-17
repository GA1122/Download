PrintingContext::Result PrintingContext::NewPage() {
  if (abort_printing_)
    return CANCEL;
  DCHECK(in_print_job_);

  NOTIMPLEMENTED();

#ifndef NDEBUG
  ++page_number_;
#endif

  return FAILED;
}
