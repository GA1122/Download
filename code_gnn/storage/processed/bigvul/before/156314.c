void MediaRecorder::ContextDestroyed(ExecutionContext*) {
  if (stopped_)
    return;

  stopped_ = true;
  stream_.Clear();
  recorder_handler_.reset();
}
