bool BackendIO::ReturnsEntry() {
  return operation_ == OP_OPEN || operation_ == OP_CREATE ||
      operation_ == OP_OPEN_NEXT;
}
