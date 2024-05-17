void MediaRecorder::requestData(ExceptionState& exception_state) {
  if (state_ == State::kInactive) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kInvalidStateError,
        "The MediaRecorder's state is '" + StateToString(state_) + "'.");
    return;
  }
  WriteData(nullptr  , 0  , true  ,
            WTF::CurrentTimeMS());
}
