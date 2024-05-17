PrintingContextCairo::~PrintingContextCairo() {
  ReleaseContext();

#if !defined(OS_CHROMEOS)
  if (print_dialog_)
    print_dialog_->ReleaseDialog();
#endif
}
