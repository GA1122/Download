bool PlatformSensorProviderLinux::StartPollingThread() {
  if (!polling_thread_)
    polling_thread_.reset(new base::Thread("Sensor polling thread"));

  if (!polling_thread_->IsRunning()) {
    return polling_thread_->StartWithOptions(
        base::Thread::Options(base::MessageLoop::TYPE_IO, 0));
  }
  return true;
}
