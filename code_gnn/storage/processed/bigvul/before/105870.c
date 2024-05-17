  void PrependCopyOf(int pos, int size) {
    target_.PrependCopyOf(data_->data() + (kDataSize - pos - size), size);
  }
