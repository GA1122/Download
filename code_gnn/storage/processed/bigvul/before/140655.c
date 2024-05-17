void OutOfProcessInstance::ScheduleCallback(int id, int delay_in_ms) {
  pp::CompletionCallback callback =
      timer_factory_.NewCallback(&OutOfProcessInstance::OnClientTimerFired);
  pp::Module::Get()->core()->CallOnMainThread(delay_in_ms, callback, id);
}
