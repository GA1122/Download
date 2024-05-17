PrintingContext::Result PrintingContext::NewPage() {
  if (abort_printing_)
    return CANCEL;
  DCHECK(in_print_job_);

  if (StartPage(context_) <= 0)
    return OnError();

#ifndef NDEBUG
  ++page_number_;
#endif

  return OK;
}
