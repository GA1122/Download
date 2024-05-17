void PlatformSensorProviderLinux::SetFileTaskRunner(
    scoped_refptr<base::SingleThreadTaskRunner> file_task_runner) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  if (!file_task_runner_)
    file_task_runner_ = file_task_runner;
}
