void PrintingContext::ResetSettings() {
#ifndef NDEBUG
  page_number_ = -1;
#endif
  dialog_box_dismissed_ = false;
  abort_printing_ = false;
  in_print_job_ = false;
}
