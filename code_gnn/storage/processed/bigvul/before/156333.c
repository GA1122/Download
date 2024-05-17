void MediaRecorder::start(ExceptionState& exception_state) {
  start(std::numeric_limits<int>::max()  , exception_state);
}
