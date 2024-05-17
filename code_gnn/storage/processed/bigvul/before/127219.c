 void UnprivilegedProcessDelegate::KillProcess(DWORD exit_code) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  channel_.reset();

  if (worker_process_.IsValid()) {
    TerminateProcess(worker_process_, exit_code);
  }
}
