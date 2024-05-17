void WaitCallback(base::WaitableEvent* event) {
  event->Signal();
}
