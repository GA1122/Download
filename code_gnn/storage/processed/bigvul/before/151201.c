  void Start(ExecutionContext* execution_context) {
    raw_data_ = SharedBuffer::Create();
    loader_->Start(execution_context, blob_);
  }
