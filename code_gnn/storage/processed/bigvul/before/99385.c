PrintingContext::PrintingContext()
    : context_(NULL),
#ifndef NDEBUG
      page_number_(-1),
#endif
      dialog_box_(NULL),
      dialog_box_dismissed_(false),
      in_print_job_(false),
      abort_printing_(false) {
}
