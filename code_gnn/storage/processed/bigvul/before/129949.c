void TargetThread::SignalThreadToFinish() {
  finish_event_.Signal();
}
