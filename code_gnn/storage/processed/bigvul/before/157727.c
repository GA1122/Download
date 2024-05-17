  explicit CloseDialogCallbackWrapper(CloseCallback callback)
      : callback_(std::move(callback)) {}
