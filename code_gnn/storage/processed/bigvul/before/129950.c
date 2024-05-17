 void TargetThread::WaitForThreadStart() {
  thread_started_event_.Wait();
}
