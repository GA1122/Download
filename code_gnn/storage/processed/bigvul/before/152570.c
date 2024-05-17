  explicit AlertCallbackFilter(
      base::RepeatingCallback<void(const base::string16&)> callback)
      : callback_(std::move(callback)) {}
