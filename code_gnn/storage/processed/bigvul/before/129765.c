void ChildThread::EnsureConnected() {
  VLOG(0) << "ChildThread::EnsureConnected()";
  base::KillProcess(base::GetCurrentProcessHandle(), 0, false);
}
