  void Run(bool dialog_was_suppressed,
           bool success,
           const base::string16& user_input) {
    if (callback_.is_null())
      return;
    std::move(callback_).Run(dialog_was_suppressed, success, user_input);
  }
