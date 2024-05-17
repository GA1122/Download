void PrintingContext::DismissDialog() {
  if (dialog_box_) {
    DestroyWindow(dialog_box_);
    dialog_box_dismissed_ = true;
  }
}
