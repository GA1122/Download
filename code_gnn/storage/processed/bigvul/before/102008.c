PrintingContext::Result PrintingContextCairo::PageDone() {
  if (abort_printing_)
    return CANCEL;
  DCHECK(in_print_job_);


  return OK;
}
