void WebSocketExperimentRunner::Cancel() {
  next_state_ = STATE_NONE;
  ChromeThread::PostTask(
      ChromeThread::IO,
      FROM_HERE,
      NewRunnableMethod(this, &WebSocketExperimentRunner::DoLoop));
}
