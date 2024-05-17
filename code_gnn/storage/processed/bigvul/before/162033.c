void BrowserChildProcessHostImpl::OnMojoError(
    base::WeakPtr<BrowserChildProcessHostImpl> process,
    scoped_refptr<base::SingleThreadTaskRunner> task_runner,
    const std::string& error) {
  if (!task_runner->BelongsToCurrentThread()) {
    task_runner->PostTask(
        FROM_HERE, base::BindOnce(&BrowserChildProcessHostImpl::OnMojoError,
                                  process, task_runner, error));
    return;
  }
  if (!process)
    return;
  HistogramBadMessageTerminated(
      static_cast<ProcessType>(process->data_.process_type));
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableKillAfterBadIPC)) {
    return;
  }
  LOG(ERROR) << "Terminating child process for bad Mojo message: " << error;

  base::debug::ScopedCrashKeyString scoped_error_key(
      bad_message::GetMojoErrorCrashKey(), error);
  base::debug::DumpWithoutCrashing();
  process->child_process_->GetProcess().Terminate(
      RESULT_CODE_KILLED_BAD_MESSAGE, false);
}
