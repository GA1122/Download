PrintingContext::Result PrintingContext::DocumentDone() {
  if (abort_printing_)
    return CANCEL;
  DCHECK(in_print_job_);

  NOTIMPLEMENTED();

  ResetSettings();
  return FAILED;
}
