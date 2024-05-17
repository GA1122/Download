bool PrinterQuery::is_valid() const {
  return worker_.get() != NULL;
}
