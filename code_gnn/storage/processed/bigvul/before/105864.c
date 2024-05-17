  void Append(int pos, int size) {
    target_.Append(data_, data_->data() + pos, size);
  }
