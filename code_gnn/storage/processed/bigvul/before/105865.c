  void AppendCopyOf(int pos, int size) {
    target_.AppendCopyOf(data_->data() + pos, size);
  }
