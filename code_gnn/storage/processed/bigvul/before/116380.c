  ~RangeTransactionServer() {
    not_modified_ = false;
    modified_ = false;
    bad_200_ = false;
  }
