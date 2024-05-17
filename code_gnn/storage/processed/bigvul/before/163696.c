  CloseDialogCallbackWrapper(
      const base::Callback<void(bool, bool, const base::string16&)> callback)
      : callback_(callback) {}
