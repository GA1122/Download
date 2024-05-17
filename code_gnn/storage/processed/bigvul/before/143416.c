  void OnError(int line_number, const base::string16& error) {
    waiter_.NotifyEvent(EVENT_ERROR);
    errors_.push_back(std::make_pair(line_number, base::UTF16ToASCII(error)));
    if (!error_callback_.is_null())
      error_callback_.Run();
  }
