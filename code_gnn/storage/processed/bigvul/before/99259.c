PrinterQuery::~PrinterQuery() {
  DCHECK(!is_print_dialog_box_shown_);
  DCHECK(!worker_.get());
  if (callback_.get()) {
    callback_->Cancel();
  }
}
