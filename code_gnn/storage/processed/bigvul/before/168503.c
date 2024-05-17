BytesConsumer::Result ReadableStreamBytesConsumer::BeginRead(
    const char** buffer,
    size_t* available) {
  *buffer = nullptr;
  *available = 0;
  if (state_ == PublicState::kErrored)
    return Result::kError;
  if (state_ == PublicState::kClosed)
    return Result::kDone;

  if (pending_buffer_) {
    DCHECK_LE(pending_offset_, pending_buffer_->length());
    *buffer = reinterpret_cast<const char*>(pending_buffer_->Data()) +
              pending_offset_;
    *available = pending_buffer_->length() - pending_offset_;
    return Result::kOk;
  }
  if (!is_reading_) {
    is_reading_ = true;
    ScriptState::Scope scope(script_state_.get());
    ScriptValue reader(script_state_.get(),
                       reader_.NewLocal(script_state_->GetIsolate()));
    DCHECK(!reader.IsEmpty());
    ReadableStreamOperations::DefaultReaderRead(script_state_.get(), reader)
        .Then(OnFulfilled::CreateFunction(script_state_.get(), this),
              OnRejected::CreateFunction(script_state_.get(), this));
  }
  return Result::kShouldWait;
}
