void MediaRecorder::start(int time_slice, ExceptionState& exception_state) {
  if (state_ != State::kInactive) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kInvalidStateError,
        "The MediaRecorder's state is '" + StateToString(state_) + "'.");
    return;
  }
  state_ = State::kRecording;

  if (!recorder_handler_->Start(time_slice)) {
    exception_state.ThrowDOMException(DOMExceptionCode::kUnknownError,
                                      "The MediaRecorder failed to start "
                                      "because there are no audio or video "
                                      "tracks available.");
    return;
  }
  ScheduleDispatchEvent(Event::Create(event_type_names::kStart));
}
