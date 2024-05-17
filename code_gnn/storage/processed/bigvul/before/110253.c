int NaClIPCAdapter::Send(const char* input_data, size_t input_data_len) {
  base::AutoLock lock(lock_);

  if (input_data_len > IPC::Channel::kMaximumMessageSize) {
    ClearToBeSent();
    return -1;
  }

  const char* current_message;
  size_t current_message_len;
  bool did_append_input_data;
  if (locked_data_.to_be_sent_.empty()) {
    current_message = input_data;
    current_message_len = input_data_len;
    did_append_input_data = false;
  } else {

    COMPILE_ASSERT(IPC::Channel::kMaximumMessageSize < (UINT_MAX / 2),
                   MaximumMessageSizeWillOverflow);
    size_t new_size = locked_data_.to_be_sent_.size() + input_data_len;
    if (new_size > IPC::Channel::kMaximumMessageSize) {
      ClearToBeSent();
      return -1;
    }

    locked_data_.to_be_sent_.append(input_data, input_data_len);
    current_message = &locked_data_.to_be_sent_[0];
    current_message_len = locked_data_.to_be_sent_.size();
    did_append_input_data = true;
  }

  switch (GetBufferStatus(current_message, current_message_len)) {
    case MESSAGE_IS_COMPLETE: {
      bool success = SendCompleteMessage(current_message, current_message_len);
      ClearToBeSent();
      return success ? static_cast<int>(input_data_len) : -1;
    }
    case MESSAGE_IS_TRUNCATED:
      if (!did_append_input_data)
        locked_data_.to_be_sent_.append(input_data, input_data_len);
      return static_cast<int>(input_data_len);
    case MESSAGE_HAS_EXTRA_DATA:
    default:
      ClearToBeSent();
      return -1;
  }
}
