void DaemonProcess::OnPermanentError() {
  DCHECK(caller_task_runner()->BelongsToCurrentThread());
  Stop();
}
