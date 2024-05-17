  void Run(bool dialog_was_suppressed,
           bool success,
           const base::string16& user_input) {
    if (already_fired_)
      return;
    already_fired_ = true;
    callback_.Run(dialog_was_suppressed, success, user_input);
  }
