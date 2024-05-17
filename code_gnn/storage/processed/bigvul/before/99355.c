void PrintingContext::Cancel() {
  abort_printing_ = true;
  in_print_job_ = false;

  NOTIMPLEMENTED();
}
