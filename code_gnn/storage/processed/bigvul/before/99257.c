bool PrinterQuery::is_callback_pending() const {
  return callback_.get() != NULL;
}
