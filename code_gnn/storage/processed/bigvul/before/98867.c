void WebSocketExperimentTask::SetTimeout(int64 deadline_ms) {
  bool r = ChromeThread::PostDelayedTask(
      ChromeThread::IO,
      FROM_HERE,
      method_factory_.NewRunnableMethod(&WebSocketExperimentTask::OnTimedOut),
      deadline_ms);
  DCHECK(r) << "No IO thread running?";
}
