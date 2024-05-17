void PrintingContext::ResetSettings() {
  if (context_ != NULL) {
    DeleteDC(context_);
    context_ = NULL;
  }
  settings_.Clear();
  in_print_job_ = false;

#ifndef NDEBUG
  page_number_ = -1;
#endif
}
