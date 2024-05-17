  void Prepend(int pos, int size) {
    target_.Prepend(data_, data_->data() + kDataSize - pos - size, size);
  }
